#include <cstdlib>



// Common headers
#include "System.h"
#include "Configurer.h"
#include "Chrono.h"

// entities
#include "lte-ae/LteEnb.h"
#include "Cluster/LteCluster.h"
#include "User.h"


#include "RxPower.h"
#include "ParallelLteScan.h"
#include "PrintUsers.h"
#include "SquareRandomLocator.h"
#include "StaticLocator.h"
#include "CallUp.h"
#include "UlAccessSelection/UplinkConn.h"
#include "UlAccessSelection/UplinkPowSimple.h"
#include "UlAccessSelection/UplinkPowCl.h"
#include "UlAccessSelection/UplinkEffectiveSinr.h"
#include "UlAccessSelection/UplinkPowerLP.h"
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
    {//==|----> define logging levels
        //    LOG_SET_ALL_LEVEL(LogLevel::ALL)
        //        LOG_SET_LEVEL("LteCell", LogLevel::INFO);
        //        LOG_SET_LEVEL("UplinkConn", LogLevel::WARNING);
        //    LOG_SET_LEVEL("RxPower", LogLevel::INFO)
        //        LOG_SET_LEVEL("UplinkPowSimple", LogLevel::INFO);
        //        LOG_SET_LEVEL("UplinkPowCl", LogLevel::INFO);
        LOG_SET_LEVEL("UplinkEffectiveSinr", LogLevel::INFO);
        LOG_SET_LEVEL("UplinkPowerLP", LogLevel::DEBUG);
    }
    {//==|----> create network types
        net.Type<User, UserConf>("USER");
        net.Type<LteUe, LteUeConf>("LTE_UE", USERS_CONF);
        //
        net.Type<LteCell, LteCellConf>("MACRO_CELL", EnbType::MACRO, ENBS_CONF);
        net.Type<LteEnb, LteEnbConf>("MACRO", EnbType::MACRO, ENBS_CONF);
        net.Type<LteCell, LteCellConf>("PICO_CELL", EnbType::PICO, ENBS_CONF);
        net.Type<LteEnb, LteEnbConf>("PICO", EnbType::PICO, ENBS_CONF);
        net.Type<Service, ServConf>("GENERIC_SERVICE", ServType::GENERIC, USERS_CONF);
    }
    {//==|----> global deployment
        net.SetTreeBase("USER",{"USER", "NUMBER"});
        net.SetTreeBase("MACRO",{"MACRO", "NUMBER"});
        net.SetTreeBase("PICO",{"PICO", "NUMBER"});
    }
    {//==|----> instantiaton
        net.SetConfig<gnsm::Configurer>(GLOB_CONF);
        net.Deploy();
        //    net.PrintInstances(gnsm::ts::NAME::FULL);
    }
    {//==|----> aggregate type
        net.Aggregate("ENBS",{"MACRO", "PICO"});
    }
    {//==|----> locate the eNBs only at start
        net.Initializer<EnbHexLocator>({"MACRO"}, units::m(0), units::m(500), 2);
        net.Initializer<EnbRandLocator>({"PICO"}, units::m(-400), units::m(400));
    }
    {//==|----> set services pattern
        net.Action<ServiceAlwaysOn>({"USER::*::GENERIC_SERVICE"});
    }
    {//==|----> set valid state at the begining of the snapshot
        net.Action<UserCallUp>({"USER"});
        net.Action<EnbCallUp>({"ENBS"});
    }
    {//==|----> locate the users
        //        net.Action<SquareRandomLocator>({"USER"}, units::m(-600), units::m(600));
        net.Action<StaticLocator>({"USER"});
    }
    {//==|----> scan environment
        //    net.Action<ParallelLteScan>({"USER", "ENBS"}, AntennaType_e::HV, PropType_e::FULL);
        net.Action<RxPower>({"USER", "ENBS"}, AntennaType_e::HONLY, PropType_e::LOS);
    }
    {//==|----> access selection
        net.Action<UplinkConn>({"USER"}, UplinkConn::Mode::CRE, units::dB(6.0), units::dB(3.0), units::dB(0.0));
    }
    {//==|----> calculate transmission power
        //    net.Action<PrintUsers>({"USER"}, PrintUsers::PrintType::CONSOLE);
        //    net.Action<UplinkPowSimple>({"USER"}, UplinkPowSimple::PcMode::OlPc);
        //    net.Action<UplinkPowSimple>({"USER"}, UplinkPowSimple::PcMode::NoPc);
        //        net.Action<UplinkPowCl>({"USER"});
        net.Action<UplinkPowerLP>({"USER"});
    }
    {//==|----> define interference level according to previous snapshot!!
        net.Action<UplinkEffectiveSinr>({"USER"});
    }
    {//==|----> get results
        net.Action<PrintUsers>({"USER"}, PrintUsers::PrintType::EFF_SINR);
    }
    auto start = Now();
    net.Run();
    UINFO(Time2ms(Now() - start), " ms");
    return 0;
}