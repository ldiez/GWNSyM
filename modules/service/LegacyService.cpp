#include "LegacyService.h"
#include "CoreUtils.h"
#include "Log.h"

LOG_REGISTER_MODULE("LegacyService");

void
LegacyService::SetConfiguration(ServConf const& conf)
{
    BEG;
    m_conf = &conf;
//    m_capacity = conf->ReadCapacity();
    END;
}

void
LegacyService::SetIntention(bool intention)
{
    BEG;
    m_intention = intention;
    END;
}

void
LegacyService::SetConnectionState(ConnectionState_e ce)
{
    BEG;
    m_connection = ce;
    auto update_ = TransitionMap_s.at(std::make_tuple(m_state, m_intention, m_connection));
    m_state = update_.first;
    m_lastTransition = update_.second;
    m_stats.UpdateInfo(m_state, update_.second);
    END;
}

Traffic
LegacyService::RequiredCapacity(void)
{
    BEG;
    return ( m_intention && m_state != ServiceState_e::DROPPED
            && m_state != ServiceState_e::REJECTED ? m_capacity : Traffic());
    END;
}

ServiceStats const&
LegacyService::ReadStats(void) const
{
    BEG END;
    return m_stats;
}