#include "SimpleUplink.h"
#include "Log.h"
#include "SimpleSelection.h"

LOG_REGISTER_MODULE("SimpleUplink");

SimpleUplink::SimpleUplink()
{
    BEG END;
}

void 
SimpleUplink::operator () ( gnsm::Vec_t<User> users )
{
    BEG;
    /**
     * Each user has a target SINR.
     * SINR = P0 * PL(1-\alpha)/(NI)
     */
    
    for (auto& user_ : users)
    {
        Select(user_);
    }
    
    
    
    END;
}

void 
SimpleUplink::Select (gnsm::Ptr_t<User>& user)
{
    BEG;
    
    auto sinrTh_ = user->GetLteDev()->ReadConfiguration().UlSinrTh();
    
    auto sinrLin_ = units::Convert<units::LinPref::Ref>(sinrTh_).RawVal();
    
    auto nrbs_ = Nrbs(sinrLin_, user);
    
    // Get best cell
    auto& cells_ = user->GetLteDev()->ReadOrderedCellsUl(); 
    MSG_ASSERT (!cells_.empty(), "User has no alternatives for UL");
    auto cell_ = cells_.begin()->m_cell;
    
    auto plLin_ = units::Convert<units::LinPref::Ref>(cells_.begin()->m_pl).RawVal();
    auto nfLin_ = units::Convert<units::LinPref::Ref>(cell_->ReadConfig().GetNoiseFigure()).RawVal();
    auto imLin_ = units::Convert<units::LinPref::Ref>(LTE::UlIm_s).RawVal();
    
    
    Power rawVal_(units::MilliWatt(LTE::RbAwgnMw_s * nfLin_ * imLin_ * 
    std::pow(plLin_, (1.0 - LTE::UlAlpha_s)) * sinrLin_ * nrbs_));
    user->GetLteDev()->UlConnect({cell_}, {rawVal_, nrbs_});
    END;
}

double 
SimpleUplink::Nrbs(double sinr, gnsm::Ptr_t<User>& user)
{
    BEG;
    
    auto uldem_ = user->GetConnectionManager()->UlDemand();
    
    auto nrbs_ = uldem_.GetKbps()/ (LTE::RbBw_s.RawVal() * std::log2(1.0 + sinr));
    
    INFO ("Number of RBS: ", nrbs_); 
    
    END;
    return nrbs_;
}