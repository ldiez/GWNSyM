#include "SaUtility.h"
#include "Log.h"

LOG_REGISTER_MODULE("SaUtility");

SaUtility::SaUtility(void)
{
    BEG END;
}

SaUtility::~SaUtility()
{
    BEG END;
}

void
SaUtility::SetNetState(TranslatedVec_t const& vec)
{
    BEG;
    m_trVec = &vec;
    END;
}

double
SaUtility::Cost(InfoCarrier& info)
{
    BEG;
    MSG_ASSERT(info.GetNumUnits() == m_trVec->size(), "Size of the local information [" 
            + std::to_string(m_trVec->size()) + "] and info carrier [" 
            + std::to_string(info.GetNumUnits()) + "] do not match");
    END;
    return DoCost(info);

}

double
SaUtility::DoCost(InfoCarrier& ic)
{
    BEG;
    auto cost_(0.0);
    auto ctr_ = 0u;
    for (auto& item_ : *m_trVec)
    {
        auto acc_ = 0.0;
        auto ctr2_ = 0u;
        for (auto& it_ : item_.m_interference)
        {
            if (ctr2_ != ctr_)
            {
                acc_ += it_ * ic.GetAssignment(ctr2_);
            }
            ++ctr2_;
        }

        auto target_ = ic.GetTarget(ctr_);
        auto perf_ = GetPerformance(ic.GetAssignment(ctr_), item_.m_pow, item_.m_interFloor, acc_);
        auto diff_ = perf_.first - target_;
        ic.SetThputExcess(ctr_, diff_);
        ic.SetSeff(ctr_, perf_.second);
        ic.SetMutualInter(ctr_, acc_);
        cost_ += std::abs(diff_);

        INFO("User ", ctr_, " with assignment ", ic.GetAssignment(ctr_), " seff ", perf_.second
             , " and diff ", diff_);
        ++ctr_;
    }
    END;
    return cost_;
}

std::pair<double, double>
SaUtility::GetPerformance(double assign, double pow, double addedNoise, double inter) const
{
    BEG END;

    auto seff_ = std::log2(1.0 + pow / (LTE::RbAwgnMwEff_s + addedNoise + inter));
//    seff_ = seff_ < 0.0 ? 0.0 : seff_;
    
    auto thput_ = assign * InfoCarrier::ResFactor * seff_;
    return
    {
        thput_, seff_
    };
}
