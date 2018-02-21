#include "SimpleUplink.h"
#include "Log.h"
#include "SimpleSelection.h"

LOG_REGISTER_MODULE("SimpleUplink");

SimpleUplink::SimpleUplink() {
    BEG END;
}

void
SimpleUplink::operator()(gnsm::Vec_t<User> users) {
    BEG;
    /**
     * Each user has a target SINR.
     * SINR = P0 * PL(1-\alpha)/(NI)
     */
    for (auto& user_ : users) {
        Select(user_);
    }
    END;
}

void
SimpleUplink::Select(gnsm::Ptr_t<User>& user) {
    BEG;

    auto sinrTh_ = user->GetLteDev()->ReadConfiguration().UlSinrTh();
    auto sinrLin_ = units::Convert<units::LinPref::Ref>(sinrTh_).RawVal();
    auto nrbs_ = Nrbs(sinrLin_, user);

    // Get best cell
    auto& cells_ = user->GetLteDev()->ReadOrderedCellsUl();
    MSG_ASSERT(!cells_.empty(), "User has no alternatives for UL");
    auto cellInfo_ = cells_.at(0);
    auto cell_ = cellInfo_.m_cell;

    using namespace units;
    auto plLin_ = Convert<LinPref::Ref>(cellInfo_.m_pl).RawVal();
    auto nfLin_ = Convert<LinPref::Ref>(cell_->ReadConfig().GetNoiseFigure()).RawVal();
    auto imLin_ = Convert<LinPref::Ref>(LTE::UlIm_s).RawVal();
    auto rxGain_ = Convert<LinPref::Ref>(cell_->ReadConfig().GetRxGain()).RawVal();
    

    auto noise = LTE::RbAwgnMw_s * nfLin_ * imLin_;
    auto gains = std::pow(plLin_, (1.0 - LTE::UlAlpha_s)) * rxGain_;
    Power txPow_(units::MilliWatt(noise * sinrLin_ / gains));
    
    DBG ("Connecting to eNB ", cell_->ReadEnbId(), " - ", cell_->ReadId(), 
            " with PL ", cellInfo_.m_pl.RawVal() , " dB");
    DBG ("Noise power ", LTE::RbAwgnMw_s, " mW");
    DBG ("Noise figure ", cell_->ReadConfig().GetNoiseFigure().RawVal(), " dB");
    DBG ("Interference margin ", LTE::UlIm_s.RawVal(), " dB");
    DBG ("Total gain ", gains);
    DBG ("alpha ", LTE::UlAlpha_s);
    DBG ("#NRBs ", nrbs_);
    DBG ("Tx power  ", txPow_.GetMilliWatt(), " mW");
    if (cell_->AddUlUser(user->ReadId(), nrbs_)) {
        user->GetLteDev()->UlConnect({cell_},
        {
            txPow_, nrbs_
        });
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