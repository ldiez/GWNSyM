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
#include "UlAccessSelection/SimpleUplink.h"
#include "PosRaster-2d.h"
#include "lte-ae/EnbHexLocator.h"
#include "lte-ae/EnbRandLocator.h"
#include "MoveAwayLocator.h"

// configuration paths
#define GLOB_CONF "./config-files/DEV/GLOB_CONF.cfg"
#define ENBS_CONF "./config-files/DEV/ENBS.xml"
#define USERS_CONF "./config-files/DEV/USER.xml"

int
main(void) {
    gnsm::System net;

    //    LOG_SET_ALL_LEVEL(LogLevel::ALL)
    //    LOG_SET_LEVEL("PrintUsers", LogLevel::WARNING);
    //    LOG_SET_LEVEL("SimpleUplink", LogLevel::ALL);
    //    LOG_SET_LEVEL("LteUe", LogLevel::INFO)
    //    LOG_SET_LEVEL("RxPower", LogLevel::INFO)
    LOG_SET_LEVEL("PrintUsers", LogLevel::INFO)
    //
    net.Type<User, UserConf>("USER");
    net.Type<LteUe, LteUeConf>("LTE_UE", USERS_CONF);
    //
    net.Type<LteCell, LteCellConf>("MACRO_CELL", EnbType::MACRO, ENBS_CONF);
    net.Type<LteEnb, LteEnbConf>("MACRO", EnbType::MACRO, ENBS_CONF);
    net.Type<LteCell, LteCellConf>("PICO_CELL", EnbType::PICO, ENBS_CONF);
    net.Type<LteEnb, LteEnbConf>("PICO", EnbType::PICO, ENBS_CONF);
    net.Type<Service, ServConf>("GENERIC_SERVICE", ServType::GENERIC, USERS_CONF);
    // Global deployment
    net.SetTreeBase("USER",{"USER", "NUMBER"});
    net.SetTreeBase("MACRO",{"MACRO", "NUMBER"});
    //    net.SetTreeBase("PICO",{"PICO", "NUMBER"});
    net.SetConfig<gnsm::Configurer>(GLOB_CONF);
    //
    net.Deploy();
    net.PrintInstances();
    //
    net.Aggregate("ENBS",{"MACRO", "PICO"});
    net.Initializer<EnbHexLocator>({"MACRO"}, units::m(0), units::m(450), 1);
    //    net.Initializer<EnbRandLocator>({"PICO"}, units::m(-600), units::m(600));
    //    net.Action<PosRaster_2d>({"USER"}, 10, 1200, -600);
    net.Action<UserCallUp>({"USER"});
    net.Action<EnbCallUp>({"ENB"});
    net.Action<MoveAwayLocator>({"USER"}, Position{0, 0, 1.5}, units::m(1));
    net.Action<RxPower>({"USER", "ENBS"}, AntennaType_e::HV, PropType_e::FULL);
    net.Action<PrintUsers>({"USER"}, PrintUsers::PrintType::RSRPDIST);
    net.Action<SimpleUplink>("USER", SimpleUplink::Mode::RSRP);
    // calculate POWER
    // calculate affective SINR
    
    auto start_ = std::chrono::steady_clock::now();
    net.Run();
    std::cout << std::chrono::duration_cast <std::chrono::milliseconds>
            (std::chrono::steady_clock::now() - start_).count() << "ms" << std::endl;
    return 0;
}