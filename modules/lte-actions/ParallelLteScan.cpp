#include "ParallelLteScan.h"
#include "Log.h"

#include "ThreadPool.h"

LOG_REGISTER_MODULE ( "ParallelLteScan" );

ParallelLteScan::ParallelLteScan(AntennaType_e ant, PropType_e prop)
: m_antennaType(ant)
, m_propType(prop)
{
    BEG END;
}

void
ParallelLteScan::operator() ( gnsm::Vec_t<User> users, gnsm::Vec_t<LteEnb> enbs )
{
    BEG;
    ParThreadPool<gnsm::Ptr_t<User>>::Function_t func_ = 
            std::bind(&ParallelLteScan::DoUserScan, this, std::placeholders::_1, std::cref(enbs));
    ParThreadPool<gnsm::Ptr_t<User>> thPool_(users, func_);
    END;
}

void
ParallelLteScan::DoUserScan ( gnsm::Ptr_t<User> user, gnsm::Vec_t<LteEnb> enbs )
{
    BEG;
    INFO ("Performing scanning of user ", user->ReadId());
    if ( user->IsActive() )
    {
        
        for ( auto& item_ : enbs )
        {
            
            LteRxPower(user, item_, m_antennaType, m_propType);
        }
    }
    END;
}

 



