#include <cstdlib>
#include <chrono>

// Common headers
#include "System.h"
#include "Configurer.h"

// entities
#include "lte-ae/LteEnb.h"
#include "Cluster/LteCluster.h"
#include "User.h"

#include "RxPower.h"
#include "PrintUsers.h"
#include "SquareRandomLocator.h"
#include "CallUp.h"
#include "SimpleUplink.h"

// configuration paths
#define GLOB_CONF "./config-files/DEV/GLOB_CONF.cfg"
#define ENBS_CONF "./config-files/DEV/ENBS.xml"
#define USERS_CONF "./config-files/DEV/USER.xml"

int
main(void) {
    gnsm::System net;

    LOG_SET_LEVEL("PrintUsers", LogLevel::WARNING);
    LOG_SET_LEVEL("SimpleUplink", LogLevel::DEBUG);

    net.AddType<User, UserConf>("USER");
    net.AddType<LteUe, LteUeConf>("LTE_UE", USERS_CONF);

    net.Type<LteCell, LteCellConf>("MACRO_CELL", EnbType::MACRO, ENBS_CONF);
    net.Type<LteEnb, LteEnbConf>("MACRO", EnbType::MACRO, ENBS_CONF);
    net.Type<LteCell, LteCellConf>("PICO_CELL", EnbType::PICO, ENBS_CONF);
    net.Type<LteEnb, LteEnbConf>("PICO", EnbType::PICO, ENBS_CONF);
    net.AddType<Service, ServConf>("GENERIC_SERVICE", ServType::GENERIC, USERS_CONF);
    // Global deployment
    net.SetTreeBase("USER",{"USER", "NUMBER"});
    net.SetTreeBase("MACRO",{"MACRO", "NUMBER"});
    net.SetTreeBase("PICO",{"PICO", "NUMBER"});
    net.SetConfig<gnsm::Configurer>(GLOB_CONF);

    net.Deploy();
    net.PrintInstances();

    net.Aggregate("ENBS",{"MACRO", "PICO"});

    net.Action<SquareRandomLocator>({"USER"}, units::m(100.0));
    net.Action<UserCallUp>({"USER"});
    net.Action<EnbCallUp>({"ENBS"});

    net.Action<RxPower>({"USER", "ENBS"}, AntennaType_e::HONLY, PropType_e::FULL);
    net.Action<SimpleUplink>({"USER"});
    net.Action<PrintUsers>({"USER"});
    

    auto start_ = std::chrono::steady_clock::now();
    net.Run();
    std::cout << std::chrono::duration_cast <std::chrono::milliseconds>
            (std::chrono::steady_clock::now() - start_).count() << "ms" << std::endl;
    return 0;
}