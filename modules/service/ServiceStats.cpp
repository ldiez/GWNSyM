#include "ServiceStats.h"
#include "Log.h"

LOG_REGISTER_MODULE ( "ServiceStats" );

void
ServiceStats::UpdateInfo ( ServiceState_e const& state, Transition_e const& transit )
{
    BEG;

    if ( transit == Transition_e::Idle2Rejected )
    {
        ++m_totalServices;
        ++m_rejections;
    }
    else if ( transit == Transition_e::Idle2Active )
    {
        ++m_totalServices;
    }
    else if ( transit == Transition_e::Active2Dropped )
    {
        ++m_drops;
    }

    if ( state == ServiceState_e::ACTIVE )
    {
        ++m_totalOnSteps;
    }
    END;
}

std::uint32_t
ServiceStats::GetTotalServices ( void ) const
{
    BEG END;
    return m_totalServices;
}

std::uint32_t
ServiceStats::GetDrops ( void ) const
{
    BEG END;
    return m_drops;
}

std::uint32_t
ServiceStats::GetRejections ( void ) const
{
    BEG END;
    return m_rejections;
}

std::uint32_t
ServiceStats::GetTotalTimeUp ( void ) const
{
    BEG END;
    return m_totalOnSteps;
}