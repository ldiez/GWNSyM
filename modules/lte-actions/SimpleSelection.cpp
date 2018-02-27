#include "SimpleSelection.h"
#include "Log.h"

LOG_REGISTER_MODULE("SimpleSelection");

SimpleSelection::SimpleSelection(std::uint32_t maxCells, double load)
: m_maxCells(maxCells)
, m_load(load) {
    BEG;
    END;
}

void
SimpleSelection::operator()(gnsm::Ptr_t<User> user) {
    BEG;
    auto connMan_ = user->GetConnectionManager();
    if (!user->IsActive()) {
        return;
    }
    INFO("Selecting user ", user->GetId());
    auto ueDev_ = user->GetLteDev();
    m_temp.m_dl = connMan_->DlDemand();

    CalculateSinr(ueDev_);
    CalculateResources();

    INFO("DL demand ", m_temp.m_dl.GetKbps(), "Kbps");
    INFO("SINR ", m_temp.m_sinr.SinrLog(), "dB");
    INFO("DL resources ", m_temp.m_rbDl);
    LteUe::DlConn dlConn_{m_temp.m_dl, m_temp.m_rbDl};
    ueDev_->DlConnect(m_temp.m_selectedCells, dlConn_);
    connMan_->Connect(ueDev_);
    m_temp = TempInfo();
    END;
}

void
SimpleSelection::CalculateSinr(gnsm::Ptr_t<LteUe> ue) {
    BEG;
    auto& cells_ = ue->GetOrderedCellsDl();
    LteUe::SensedValues_t serving_;
    LteUe::SensedValues_t interfering_;

    auto ctr_ = 0u;

    for (auto& item_ : cells_) {
        if (ctr_ < m_maxCells && item_.m_cell->GetDlFreeResources() > 0) {
            serving_.push_back(item_);
            ++ctr_;
        } else {
            interfering_.push_back(item_);
        }
    }

    DBG("Final serving size ", serving_.size());
    DBG("Final interfering size ", interfering_.size());
    DBG("Final total size ", cells_.size());
    DoSinr(serving_, interfering_);
    END;
}

void
SimpleSelection::DoSinr(LteUe::SensedValues_t serving, LteUe::SensedValues_t interfering) {
    BEG;
    Power signal_;
    Power interference_;

    for (auto& item_ : serving) {
        signal_ += item_.m_rsrp;
    }

    for (auto& item_ : interfering) {
        interference_ += item_.m_rsrp;
    }

    INFO("Total signal ", signal_.GetDbm(), "dBm");
    INFO("Total interference ", interference_.GetDbm(), "dBm");

    Sinr sinr_(Bandwidth(LTE::RbBw_s), signal_, interference_);
    INFO("Sinr ", sinr_.SinrLog(), "dB");
    m_temp.m_sinr = sinr_;
    END;
}

void
SimpleSelection::CalculateResources(void) {
    BEG;
    m_temp.m_rbDl = m_temp.m_dl.GetKbps() / (LTE::BwEff_s * LTE::RbBw_s.RawVal() *
            std::log2(1 + m_temp.m_sinr.SinrLin() / LTE::SinrEff_s));
    END;
}