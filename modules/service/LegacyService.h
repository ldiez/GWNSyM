#ifndef LEGACYSERVICE_H
#define LEGACYSERVICE_H

#include "ServiceStateMachine.h"
#include "ServiceStats.h"
#include "ServConf.h"

/**
 * \description This class implements a simple ON/OFF service according to the state machine defined
 * in \ServiceStateMachine
 */
class LegacyService
{
public:
    void SetConfiguration ( ServConf const& conf );
    
    /**
     * \brief Set intention by the corresponding updater
     * \param intention --> For this service the intention is ON/OFF
     * \return --
     */
    void SetIntention ( bool intention );
    
    /**
     * \brief Set the current connection state
     * \param ce
     * \return --
     */
    void SetConnectionState(ConnectionState_e ce);

    /**
     * \brief Provide required capacity for the service according to the service configuration, 
     * previous state and current intention
     * \param <-- Required capacity
     * \return 
     */
    Traffic RequiredCapacity(void);

    /**
     * \brief Provide reading access to the statistics 
     * \return <-- Const ref to stats
     */
    ServiceStats const& GetStats(void) const;

private:

    Transition_e m_lastTransition;
    ServiceStats m_stats;
    Traffic m_capacity;


    bool m_intention = false;
    ServiceState_e m_state = ServiceState_e::IDLE;
    ConnectionState_e m_connection = ConnectionState_e::Connected;

    const ServConf* m_conf = nullptr;
};

#endif /* LEGACYSERVICE_H */

