#ifndef SERVICESTATS_H
#define	SERVICESTATS_H

#include "ServiceStateMachine.h"

/**
 * \description This class stores some statistics related to the state machine associated:
 * - Total number of services initiated
 * - Total number of services dropped
 * - Total number of services rejected
 * - Total time that the service is running (from all sessions)
 */
class ServiceStats
{
public:
    /**
     * \brief Update the internal information according the current state transition
     * \param state -->
     * \param transit -->
     * \return --
     */
    void UpdateInfo ( ServiceState_e const& state, Transition_e const& transit );

    /**
     * \brief Get total number of services either rejected, dropped or success
     * \return <-- Number of services 
     */
    std::uint32_t GetTotalServices ( void ) const;

    /**
     * \brief Give the number of drops for this service, i.e. transitions from ACTIVE to DROPPED
     * \return <-- Number of drops
     */
    std::uint32_t GetDrops ( void ) const;

    /**
     * \brief Get the number of rejections for this service, i.e. transitions from IDLE to REJECTED
     * \return <-- Number of rejections
     */
    std::uint32_t GetRejections ( void ) const;
    
    /**
     * \brief Number of steps that the service is up
     * \return <-- Number of steps up
     */
    std::uint32_t GetTotalTimeUp ( void ) const ;
private:

    std::uint16_t m_rejections = 0u;
    std::uint16_t m_totalServices = 0u;
    std::uint16_t m_drops = 0u;

    std::uint32_t m_totalOnSteps = 0u;
};

#endif	/* SERVICESTATS_H */

