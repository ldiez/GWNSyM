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
    auto cellInfo_ = user->GetLteDev()->GetOrderedCellDl(0);
    auto cell_ = cellInfo_.m_cell;
    if (true) {//cell_->AddUlUser(user->GetId(), nrbs_)) {
        user->GetLteDev()->UlConnect({cell_}, nrbs_);
        user->GetLteDev()->DlConnect({cell_},
        {
            Traffic(user->GetConnectionManager()->DlDemand()), nrbs_
        });
    }
    END;
}

void
UplinkConn::SelectPl(gnsm::Ptr_t<User>& user) {
    BEG;
    auto nrbs_ = Nrbs(user);
    auto cellInfo_ = user->GetLteDev()->GetOrderedCellUl(0);
    auto cell_ = cellInfo_.m_cell;
    if (true) {//cell_->AddUlUser(user->GetId(), nrbs_)) {
        user->GetLteDev()->UlConnect({cell_}, nrbs_);
        user->GetLteDev()->DlConnect({cell_},
        {
            Traffic(user->GetConnectionManager()->DlDemand()), nrbs_
        });
    }
    END;
}

void
UplinkConn::SelectCre(gnsm::Ptr_t<User>& user) {
    BEG;
//    auto info = user->GetLteDev()->GetOrderedCellsDl();
//    auto rsrp = info[0].m_rsrp;
//    if (info[0].m_type == EnbType::PICO) {
//        rsrp.Amp(m_picoBias);
//    }
    END;
}