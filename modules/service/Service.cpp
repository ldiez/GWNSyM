#include "Service.h"
#include "Log.h"
#include <iomanip>

LOG_REGISTER_MODULE("Service");

Service::Service()
: m_state(ServiceState_e::IDLE)
, m_connection(ConnectionState_e::Connected)
, m_conf(nullptr)
{
    BEG;
    m_intention = Intention::IDLE;
    END;
}

Service::~Service()
{
    BEG;
    END;
}

void
Service::SetConfiguration(ServConf const& conf)
{
    BEG;
    m_conf = &conf;
    END;
}

void
Service::SetIntention(Intention intent)
{
    BEG;
    m_intention = intent;
    INFO("");
    INFO("Service intention ", m_intention);
    UpdateState(true);
    END;
}

Intention
Service::GetIntention(void) const
{
    BEG END;
    return m_intention;
}

void
Service::SetConnectionState(ConnectionState_e ce)
{
    BEG;
    if (m_intention == Intention::IDLE)
    {
        return;
    }
    m_connection = ce;
    UpdateState();
    END;
}

Traffic
Service::DlDemand(void) const
{
    BEG;

    if (m_intention != Intention::ACTIVE ||
            !(m_state == ServiceState_e::IDLE || m_state == ServiceState_e::ACTIVE))
    {
        END;
        INFO("Service without DL demand");
        return Traffic(units::kbps(0.0));
    }
    END;
    INFO("Service with DL demand ", m_conf->GetDlCapacity().GetKbps(), "Kbps");
    return m_conf->GetDlCapacity();
}

Traffic
Service::UlDemand(void) const
{
    BEG;

    if (m_intention != Intention::ACTIVE ||
            !(m_state == ServiceState_e::IDLE || m_state == ServiceState_e::ACTIVE))
    {
        END;
        return Traffic(units::kbps(0.0));
    }
    END;
    return m_conf->GetUlCapacity();
}

void
Service::UpdateState(bool justIntention)
{
    BEG;
    
    /**
     * If the state is idle 
     * - Intention active ==> Demand will be notified
     * - Intention idle ==> Demand is 0
     */
    
    if ((justIntention && (m_state != ServiceState_e::IDLE)) || !justIntention)
    {
        bool inten_ = m_intention == Intention::IDLE ? false : true;
        auto update_ = TransitionMap_s.at(std::make_tuple(m_state, inten_, m_connection));
        m_state = update_.first;
        m_stats.UpdateInfo(m_state, update_.second);
        INFO("Service transition ", update_.second);
    }
    INFO("Service state ", m_state);

    END;
}