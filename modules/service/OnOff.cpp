#include "OnOff.h"
#include "Log.h"

LOG_REGISTER_MODULE("OnOff");

OnOff::OnOff(double on, double off)
: m_on(on)
, m_off(off)
, m_gen(std::random_device()())
{
    BEG;
    std::exponential_distribution<> dist_(1.0 / m_off);
    END;
}

void
OnOff::operator()(gnsm::Ptr_t<Service> serv)
{
    BEG;

    auto iter_ = m_track.find(serv);

    if (iter_ != m_track.end())
    {
        auto check_ = m_track.insert(ServicesTrack_t::value_type{serv, 0u});
        MSG_ASSERT (check_.second, "Bad service insertion");
        DoUpdate(check_.first);
        
    }
    else
    {
        DoUpdate(iter_);
    }


    END;
}

void
OnOff::DoUpdate (ServicesTrack_t::iterator iter)
{
    BEG;
    auto backCtr_ = iter->second;
    auto serv_ = iter->first;
    if (backCtr_ == 0u)
    {
        if (serv_->GetIntention() == Intention::IDLE)
        {
            std::exponential_distribution<> dist_(1.0 / m_on);
            backCtr_ = std::round(dist_(m_gen));
            serv_->SetIntention(Intention::ACTIVE);
        }
        else
        {
            std::exponential_distribution<> dist_(1.0 / m_off);
            backCtr_ = std::round(dist_(m_gen));
            serv_->SetIntention(Intention::IDLE);
        }
    }
    else
    {
        --backCtr_;
    }
    
    iter->second = backCtr_;
    END;
}