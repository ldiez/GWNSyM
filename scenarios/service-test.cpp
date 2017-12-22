#include <cstdlib>
#include "Service.h"
#include "Log.h"


#define USERS_CONF "./config-files/USER.xml"

int
main(int argc, char** argv)
{
    LOG_SET_LEVEL("ServConf", LogLevel::ALL);
    LOG_SET_LEVEL("Service", LogLevel::INFO);
    ServConf sc_(ServType::GENERIC, USERS_CONF);
    Service serv_;
    serv_.SetConfiguration(sc_);
    serv_.SetIntention(Intention::ACTIVE);
    serv_.SetConnectionState(ConnectionState_e::Connected);
    
    serv_.SetIntention(Intention::ACTIVE);
    serv_.SetConnectionState(ConnectionState_e::Connected);
    
    serv_.SetIntention(Intention::IDLE);
//    serv_.SetConnectionState(ConnectionState_e::Disconnected);
    
    serv_.SetIntention(Intention::ACTIVE);
    serv_.SetConnectionState(ConnectionState_e::Connected);
    return 0;
}

