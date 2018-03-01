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
#include "ParallelLteScan.h"
#include "PrintUsers.h"
#include "SquareRandomLocator.h"
#include "CallUp.h"
#include "UlAccessSelection/UplinkConn.h"
#include "UlAccessSelection/UplinkPowSimple.h"
#include "UlAccessSelection/UplinkEffectiveSinr.h"
#include "PosRaster-2d.h"
#include "lte-ae/EnbHexLocator.h"
#include "lte-ae/EnbRandLocator.h"
#include "MoveAwayLocator.h"
#include "ServiceAlwaysOn.h"

// configuration paths
#define GLOB_CONF "./config-files/DEV/GLOB_CONF.cfg"
#define ENBS_CONF "./config-files/DEV/ENBS.xml"
#define USERS_CONF "./config-files/DEV/USER.xml"

int
main(void) {
    gnsm::System net;

    //    LOG_SET_ALL_LEVEL(LogLevel::ALL)
    //    LOG_SET_LEVEL("PrintUsers", LogLevel::WARNING);
    //    LOG_SET_LEVEL("UplinkConn", LogLevel::INFO);
    //    LOG_SET_LEVEL("RxPower", LogLevel::INFO)
    //    LOG_SET_LEVEL("UplinkPowSimple", LogLevel::INFO)
    //    LOG_SET_LEVEL("UplinkEffectiveSinr", LogLevel::INFO)
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
    net.Initializer<EnbHexLocator>({"MACRO"}, units::m(0), units::m(500), 2);
    //    net.Initializer<EnbRandLocator>({"PICO"}, units::m(-600), units::m(600));
    net.Action<ServiceAlwaysOn>({"USER::*::GENERIC_SERVICE"});
    net.Action<UserCallUp>({"USER"});
    net.Action<EnbCallUp>({"ENB"});
    // All prepared
    net.Action<SquareRandomLocator>({"USER"}, units::m(-1000), units::m(1000));
    //    net.Action<ParallelLteScan>({"USER", "ENBS"}, AntennaType_e::HV, PropType_e::FULL);
    net.Action<RxPower>({"USER", "ENBS"}, AntennaType_e::HV, PropType_e::FULL);
    net.Action<UplinkConn>({"USER"}, UplinkConn::Mode::RSRP);
    //    net.Action<PrintUsers>({"USER"}, PrintUsers::PrintType::CONSOLE);
    net.Action<UplinkPowSimple>({"USER"});

    net.Action<UplinkEffectiveSinr>({"USER"});
    net.Action<PrintUsers>({"USER"}, PrintUsers::PrintType::EFF_SINR);
    // calculate effective SINR

    auto start_ = std::chrono::steady_clock::now();
    net.Run();
    std::cout << std::chrono::duration_cast <std::chrono::milliseconds>
            (std::chrono::steady_clock::now() - start_).count() << "ms" << std::endl;
    return 0;
}