#include "UplinkConn.h"
#include "lte-user/LteUe.h"
#include "Log.h"

LOG_REGISTER_MODULE("UplinkConn");

UplinkConn::UplinkConn(Mode m, units::dB picoCre,
        units::dB microCre, units::dB macroCre)
: m_mode(m)
, m_picoBias(picoCre)
, m_microbias(microCre)
, m_macroBias(macroCre) {
    BEGEND;
}

void Assign(gnsm::Ptr_t<LteCell> c, gnsm::Ptr_t<User> u, double nrbs) {
    if (c->AddUlUser(u->GetId(), nrbs)) {
        u->GetLteDev()->UlConnect({c}, nrbs);
        u->GetLteDev()->DlConnect({c},
        {
            Traffic(u->GetConnectionManager()->DlDemand()), nrbs
        });
    } else {
        WARN ("Cell ", c->GetId(), " is full for user ", u->GetId());
    }
}

void
UplinkConn::operator()(gnsm::Vec_t<User> users) {
    BEG;
    /**
     * Each user has a target SINR.
     * SINR = P0 * PL(1-\alpha)/(NI)
     */
    if (m_mode == Mode::RSRP) {
        for (auto& user_ : users) {
            SelectRsrp(user_);
        }
    } else if (m_mode == Mode::PL) {
        for (auto& user_ : users) {
            SelectPl(user_);
        }
    } else {
        for (auto& user_ : users) {
            SelectCre(user_);
        }
    }
    END;
}

double
UplinkConn::Nrbs(gnsm::Ptr_t<User>& user) {
    BEG;
    auto sinrTh_ = user->GetLteDev()->GetConfiguration().UlSinrTh();
    auto sinr = units::Convert<units::LinPref::Ref>(sinrTh_).RawVal();
    auto uldem_ = user->GetConnectionManager()->UlDemand();
    auto nrbs_ = uldem_.GetKbps() / (LTE::RbBw_s.RawVal() * std::log2(1.0 + sinr));
    END;
    return nrbs_;
}

void
UplinkConn::SelectRsrp(gnsm::Ptr_t<User>& user) {
    BEG;
    auto nrbs_ = Nrbs(user);
    if (nrbs_ == 0) { // no traffic
        return;
    }
    auto cellInfo_ = user->GetLteDev()->GetOrderedCellDl(0);
    auto cell_ = cellInfo_.m_cell;
    Assign(cell_, user, nrbs_);
    END;
}

void
UplinkConn::SelectPl(gnsm::Ptr_t<User>& user) {
    BEG;
    auto nrbs_ = Nrbs(user);
    if (nrbs_ == 0) { // no traffic
        return;
    }
    auto cellInfo_ = user->GetLteDev()->GetOrderedCellUl(0);
    auto cell_ = cellInfo_.m_cell;
    Assign(cell_, user, nrbs_);
    END;
}

void
UplinkConn::SelectCre(gnsm::Ptr_t<User>& user) {
    BEG;
    auto nrbs_ = Nrbs(user);
    if (nrbs_ == 0) { // no traffic
        return;
    }
    auto info = user->GetLteDev()->GetOrderedCellsDl();
    std::map<Power, gnsm::Ptr_t < LteCell>, std::greater < Power>> auxOrder;

    for (auto& item : info) {
        auto rsrp = item.m_rsrp;
        switch (item.m_type) {
            case EnbType::PICO:
                rsrp.Amp(m_picoBias);
                break;
            case EnbType::MICRO:
                rsrp.Amp(m_microbias);
                break;
            case EnbType::MACRO:
            default:
                rsrp.Amp(m_macroBias);
                break;
        }
        auxOrder.insert({rsrp, item.m_cell});
    }
    
    auto cell_ = auxOrder.begin()->second;
    Assign(cell_, user, nrbs_);
    END;
}