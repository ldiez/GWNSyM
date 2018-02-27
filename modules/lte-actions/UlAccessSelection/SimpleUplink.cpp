#include "SimpleUplink.h"
#include "lte-user/LteUe.h"
#include "Log.h"

LOG_REGISTER_MODULE("SimpleUplink");

SimpleUplink::SimpleUplink(Mode m, units::dB bias)
: m_mode(m)
, m_bias(bias) {
    BEGEND;
}

void
SimpleUplink::operator()(gnsm::Vec_t<User> users) {
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
    for (auto& user_ : users) {
        Select(user_);
    }
    END;
}

double
SimpleUplink::Nrbs(double sinr, gnsm::Ptr_t<User>& user) {
    BEG;
    auto uldem_ = user->GetConnectionManager()->UlDemand();
    auto nrbs_ = uldem_.GetKbps() / (LTE::RbBw_s.RawVal() * std::log2(1.0 + sinr));
    INFO("Number of RBS: ", nrbs_);
    END;
    return nrbs_;
}

void
SimpleUplink::Select(gnsm::Ptr_t<User>& user) {
    BEG;
    //    auto sinrTh_ = user->GetLteDev()->GetConfiguration().UlSinrTh();
    //    auto sinrLin_ = units::Convert<units::LinPref::Ref>(sinrTh_).RawVal();
    //    auto nrbs_ = Nrbs(sinrLin_, user);
    //    // Get best cell
    //    auto& cells_ = user->GetLteDev()->GetOrderedCellsUl();
    //    MSG_ASSERT(!cells_.empty(), "User has no alternatives for UL");
    //    auto cellInfo_ = cells_.at(0);
    //    auto cell_ = cellInfo_.m_cell;
    //    using namespace units;
    //    auto plLin_ = Convert<LinPref::Ref>(cellInfo_.m_pl).RawVal();
    //    auto nfLin_ = Convert<LinPref::Ref>(cell_->GetConfiguration().GetNoiseFigure()).RawVal();
    //    auto imLin_ = Convert<LinPref::Ref>(LTE::UlIm_s).RawVal();
    //    auto rxGain_ = Convert<LinPref::Ref>(cell_->GetConfiguration().GetRxGain()).RawVal();
    //    auto noise = LTE::RbAwgnMw_s * nfLin_ * imLin_;
    //    auto gains = std::pow(plLin_, (1.0 - LTE::UlAlpha_s)) * rxGain_;
    //    Power txPow_(units::MilliWatt(noise * sinrLin_ / gains));
    //    //    DBG("Connecting to eNB ", cell_->GetEnb()->GetId(), " - ", cell_->GetId(),
    //    //            " with PL ", cellInfo_.m_pl.RawVal(), " dB");
    //    DBG("Noise power ", LTE::RbAwgnMw_s, " mW");
    //    DBG("Noise figure ", cell_->GetConfiguration().GetNoiseFigure().RawVal(), " dB");
    //    DBG("Interference margin ", LTE::UlIm_s.RawVal(), " dB");
    //    DBG("Total gain ", gains);
    //    DBG("alpha ", LTE::UlAlpha_s);
    //    DBG("#NRBs ", nrbs_);
    //    DBG("Tx power  ", txPow_.GetMilliWatt(), " mW");
    //
    //    if (cell_->AddUlUser(user->GetId(), nrbs_)) {
    //        user->GetLteDev()->UlConnect({cell_},
    //        {
    //            Power(units::MilliWatt(0)), nrbs_
    //        });
    //    }
    END;
}

void
SimpleUplink::SelectRsrp(gnsm::Ptr_t<User>& user) {
    BEG;
    auto sinrTh_ = user->GetLteDev()->GetConfiguration().UlSinrTh();
    auto sinrLin_ = units::Convert<units::LinPref::Ref>(sinrTh_).RawVal();
    auto nrbs_ = Nrbs(sinrLin_, user);
    // Get best cell
    auto& cells_ = user->GetLteDev()->GetOrderedCellsUl();
    MSG_ASSERT(!cells_.empty(), "User has no alternatives for UL");
    auto cellInfo_ = cells_.at(0);
    auto cell_ = cellInfo_.m_cell;
    if (cell_->AddUlUser(user->GetId(), nrbs_)) {
        user->GetLteDev()->UlConnect({cell_},
        {
            Power(units::MilliWatt(0)), nrbs_
        });
    }
    END;
}

void
SimpleUplink::SelectPl(gnsm::Ptr_t<User>& user) {
    BEG;
    END;
}

void
SimpleUplink::SelectCre(gnsm::Ptr_t<User>& user) {
    BEG;
    END;
}