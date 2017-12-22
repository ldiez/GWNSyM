#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "lte-user/LteUe.h"
#include "Service.h"
#include "Traffic.h"

/**
 * \brief This class implements a handler to connect each service
 * with each interface. 
 * It is in charge of tracking the hand-overs (vertical and horizontal),
 * just an abstraction.
 * For the time being it is LTE-only and single device
 */
class ConnectionManager
{
public:
    ConnectionManager();

    /**
     * \brief Register a new service in the manager.
     * For different types of services this function will be overloaded
     * \param serv --> Service ptr
     * \return --
     */
    void RegisterService(gnsm::Ptr_t<Service> serv);

    /**
     * \brief Register a new device in the manager
     * For different types of devices this function will be overloaded
     * \param dev --> Device
     * \return 
     */
    void RegisterDevice(gnsm::Ptr_t<LteUe> dev);

    /**
     * \brief Perform connection of all the services to a device. When it is called,
     * the device is connected to the access elements. 
     * If the device is nullptr, the services are dropped/rejected.
     * It also checks the difference between demand and traffic offered by the connection
     * 
     * \param dev --> Device ptr
     * \return --
     */
    void Connect(gnsm::Ptr_t<LteUe> dev);

    /**
     * \brief Provide the DL demand 
     * \return <-- Traffic demand
     */
    Traffic DlDemand(void) const;

    /**
     * \brief Provide the UL demand 
     * \return <-- Traffic demand
     */
    Traffic UlDemand(void) const;

    /**
     * \brief Preparation for the next iteration:
     * - Calculate DL
     * - Calculate UL
     */
    void CallUp(void);

private:

    void CalculateDemands(void);
    
    gnsm::Ptr_t<LteUe> m_lteDev;
    gnsm::Vec_t<Service> m_services;
    
    Traffic m_dlDemand;
    Traffic m_ulDemand;
};

#endif /* CONNECTIONMANAGER_H */