#ifndef SERVICE_H
#define SERVICE_H

#include <algorithm>
#include <map>
#include "ServiceStateMachine.h"
#include "ServiceStats.h"
#include "ServConf.h"

/**
 * \brief Service implementation. 
 * For the time being only on-off model is implemented assuming the 
 * values of TIA and TS are supplied by the configuration.
 * 
 */
class Service
{
public:

    Service();
    ~Service();

    /**
     * \brief Set the current intention of the service
     * \param --> Intention
     * \return --
     */
    void SetIntention(Intention intent);
    Intention GetIntention(void) const;

    void SetConfiguration(ServConf const& conf);

    /**
     * \brief Set the current connection state
     * \param ce
     * \return --
     */
    void SetConnectionState(ConnectionState_e ce);

    /**
     * \brief Provide required capacity for the service according to the service configuration, 
     * previous state and current intention
     * \param <-- Required capacity for the DownLink
     * \return 
     */
    Traffic DlDemand(void) const;

    /**
     * \brief Provide required capacity for the service according to the service configuration, 
     * previous state and current intention
     * \param <-- Required capacity for the UpLink
     * \return 
     */
    Traffic UlDemand(void) const;

private:

    /**
     * \brief Update the internal state either upon a change of the intention
     * or a change of the connection
     * \param --> Set whether or not the state change is due to the intention 
     * update. In this case, it is only updated if the service was
     * DROPPED or REJECTED to go back to IDLE state
     * \return --
     */
    void UpdateState(bool newIntention = false);

    ServiceState_e m_state; // service state
    Intention m_intention; // current intention 
    ConnectionState_e m_connection; // current connection 

    const ServConf* m_conf;
    ServiceStats m_stats;
};

#endif /* SERVICE_H */