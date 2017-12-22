#ifndef SERVICESTATEMACHINE_H
#define SERVICESTATEMACHINE_H

#include "CoreUtils.h"

/**
 * \brief 
 */
enum class Intention : std::uint8_t
{
    IDLE,
    ACTIVE
};

inline std::ostream& operator<<(std::ostream& o, Intention const& inten)
{
    o << (inten == Intention::IDLE ? " IDLE " : " ACTIVE ");
    return o;
}

/**
 * \description Store the current state of the connection for the service
 */
enum class ConnectionState_e : std::size_t
{
    Connected,
    Disconnected,
};

inline std::ostream& operator<<(std::ostream& o, ConnectionState_e const& conn)
{
    o << (conn == ConnectionState_e::Connected ? " CONNECTED " : " DISCONNECTED ");
    return o;
}

/**
 * \description Store the current state of the service according to the previous 
 * state, the intention and connection state:
 *  
 * Previous     | Intention | Connection   ==> State
 * ----------------------------------------------------
 *   IDLE       |      1    |      1        ==> ACTIVE
 *   ACTIVE     |      1    |      1        ==> ACTIVE
 *   IDLE       |      0    |      -        ==> IDLE
 *   ACTIVE     |      1    |      0        ==> DROPPED
 *   IDLE       |      1    |      0        ==> REJECTED 
 *   ACTIVE     |      0    |      -        ==> IDLE
 *   REJECTED   |      1    |      -        ==> REJECTED
 *   DROPPED    |      1    |      -        ==> DROPPED
 *   REJECTED   |      0    |      -        ==> IDLE
 *   DROPPED    |      0    |      -        ==> IDLE
 */
enum class ServiceState_e : std::size_t
{
    IDLE = 1,
    REJECTED,
    DROPPED,
    ACTIVE
};

inline std::ostream& operator<<(std::ostream& o, ServiceState_e const& state)
{
    o << (state == ServiceState_e::ACTIVE ? " ACTIVE " :
            state == ServiceState_e::DROPPED ? " DROPPED " :
            state == ServiceState_e::IDLE ? " IDLE " :
            state == ServiceState_e::REJECTED ? " REJECTED " : "UNKNOWN"
            );
    return o;
}

/**
 * \description Name of the transition to be indicated
 */
enum class Transition_e : std::size_t
{
    Idle2Idle = 1,
    Idle2Active,
    Idle2Rejected,
    Active2Active,
    Active2Idle,
    Active2Dropped,
    Dropped2Dropped,
    Dropped2Idle,
    Rejected2Rejected,
    Rejected2Idle
};

inline std::ostream& operator<<(std::ostream& o, Transition_e const& tran)
{
    o << (tran == Transition_e::Active2Active ? " ACTIVE ==> ACTIVE " :
            tran == Transition_e::Active2Dropped ? " ACTIVE ==> DROPPED " :
            tran == Transition_e::Active2Idle ? " ACTIVE ==> IDLE " :
            tran == Transition_e::Idle2Active ? " IDLE ==> ACTIVE " :
            tran == Transition_e::Idle2Rejected ? " IDLE ==> REJECTED " :
            tran == Transition_e::Idle2Idle ? " IDLE ==> IDLE " :
            tran == Transition_e::Dropped2Dropped ? " DROPPED ==> DROPPED " :
            tran == Transition_e::Dropped2Idle ? " DROPPED ==> IDLE " :
            tran == Transition_e::Rejected2Idle ? " REJECTED ==> IDLE " :
            tran == Transition_e::Rejected2Rejected ? " REJECTED ==> REJECTED " : "UNKNOWN"
            );
    return o;
}




/**
 * \description Implementation of the state machine. Given an entry with the Previous state, 
 * intention and Connection state (StateTuple_t), the TransisionMap_s returns a pair with the 
 * current state and the transition that happened
 */
using StateTuple_t = std::tuple<ServiceState_e, bool, ConnectionState_e>;
using NewState_t = std::pair<ServiceState_e, Transition_e>;

struct StatepairHasher
{

    std::size_t operator()(const StateTuple_t& p) const
    {
        return (gnsm::UnscopeEmum(std::get<0>(p)) + std::get<1>(p) +
                gnsm::UnscopeEmum(std::get<2>(p)));
    }
};

static const std::unordered_map<StateTuple_t, NewState_t, StatepairHasher> TransitionMap_s = {
    {
        std::make_tuple(ServiceState_e::IDLE, true, ConnectionState_e::Disconnected),
        std::make_pair(ServiceState_e::REJECTED, Transition_e::Idle2Rejected)
    },
    {
        std::make_tuple(ServiceState_e::IDLE, true, ConnectionState_e::Connected),
        std::make_pair(ServiceState_e::ACTIVE, Transition_e::Idle2Active)
    },
    {
        std::make_tuple(ServiceState_e::IDLE, false, ConnectionState_e::Connected),
        std::make_pair(ServiceState_e::IDLE, Transition_e::Idle2Idle)
    },
    {
        std::make_tuple(ServiceState_e::IDLE, false, ConnectionState_e::Disconnected),
        std::make_pair(ServiceState_e::IDLE, Transition_e::Idle2Idle)
    },
    {
        std::make_tuple(ServiceState_e::ACTIVE, true, ConnectionState_e::Disconnected),
        std::make_pair(ServiceState_e::DROPPED, Transition_e::Active2Dropped)
    },
    {
        std::make_tuple(ServiceState_e::ACTIVE, true, ConnectionState_e::Connected),
        std::make_pair(ServiceState_e::ACTIVE, Transition_e::Active2Active)
    },
    {
        std::make_tuple(ServiceState_e::ACTIVE, false, ConnectionState_e::Disconnected),
        std::make_pair(ServiceState_e::IDLE, Transition_e::Active2Idle)
    },
    {
        std::make_tuple(ServiceState_e::ACTIVE, false, ConnectionState_e::Connected),
        std::make_pair(ServiceState_e::IDLE, Transition_e::Active2Idle)
    },
    {
        std::make_tuple(ServiceState_e::DROPPED, false, ConnectionState_e::Connected),
        std::make_pair(ServiceState_e::IDLE, Transition_e::Dropped2Idle)
    },
    {
        std::make_tuple(ServiceState_e::DROPPED, false, ConnectionState_e::Disconnected),
        std::make_pair(ServiceState_e::IDLE, Transition_e::Dropped2Idle)
    },
    {
        std::make_tuple(ServiceState_e::DROPPED, true, ConnectionState_e::Disconnected),
        std::make_pair(ServiceState_e::DROPPED, Transition_e::Dropped2Dropped)
    },
    {
        std::make_tuple(ServiceState_e::DROPPED, true, ConnectionState_e::Connected),
        std::make_pair(ServiceState_e::DROPPED, Transition_e::Dropped2Dropped)
    },
    {
        std::make_tuple(ServiceState_e::REJECTED, true, ConnectionState_e::Connected),
        std::make_pair(ServiceState_e::REJECTED, Transition_e::Rejected2Rejected)
    },
    {
        std::make_tuple(ServiceState_e::REJECTED, true, ConnectionState_e::Disconnected),
        std::make_pair(ServiceState_e::REJECTED, Transition_e::Rejected2Rejected)
    },
    {
        std::make_tuple(ServiceState_e::REJECTED, false, ConnectionState_e::Connected),
        std::make_pair(ServiceState_e::IDLE, Transition_e::Rejected2Idle)
    },
    {
        std::make_tuple(ServiceState_e::REJECTED, false, ConnectionState_e::Disconnected),
        std::make_pair(ServiceState_e::IDLE, Transition_e::Rejected2Idle)
    }
};



#endif /* SERVICESTATEMACHINE_H */