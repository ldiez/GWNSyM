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
#include "PrintCells.h"
#include "SquareRandomLocator.h"
#include "RandomAnulusLocator.h"
#include "CallUp.h"
#include "UlAccessSelection/UplinkConn.h"
#include "UlAccessSelection/UplinkPowSimple.h"
#include "UlAccessSelection/UplinkPowCl.h"
#include "UlAccessSelection/UplinkEffectiveSinr.h"
#include "UlAccessSelection/UplinkPowerLP.h"
#include "PosRaster-2d.h"
#include "lte-ae/EnbHexLocator.h"
#include "lte-ae/EnbRandLocator.h"
#include "lte-ae/EnbFixedLocator.h"
#include "UserFixedLocator.h"
#include "UlAccessSelection/UlClosestConn.h"
#include "MoveAwayLocator.h"
#include "ServiceAlwaysOn.h"

// configuration paths
#define GLOB_CONF "./config-files/DEV/GLOB_CONF.cfg"
#define ENBS_CONF "./config-files/DEV/ENBS.xml"
#define USERS_CONF "./config-files/DEV/USER.xml"

int
main(int argc, char** argv)
{
    PrintUsers::UlcpType pctype = PrintUsers::UlcpType::LP;
    UplinkConn::Mode astype = UplinkConn::Mode::RSRP;
    std::string argPc = argv[1];
    std::string argAs = argv[2];
    if (argPc == "ol")
    {
//        UINFO("OL power control");
        pctype = PrintUsers::UlcpType::OL;
    }
    else if (argPc == "lp")
    {
//        UINFO("LP power control");
        pctype = PrintUsers::UlcpType::LP;
    }
    else if (argPc == "nopc")
    {
//        UINFO("NOPC power control");
        pctype = PrintUsers::UlcpType::NC;
    }
    else if (argPc == "cl")
    {
//        UINFO("CL power control");
        pctype = PrintUsers::UlcpType::CL;
    }
    else
    {
        MSG_ASSERT(false, "Invalid power control argument");
    }

    if (argAs == "rsrp")
    {
//        UINFO("RSRP");
        astype = UplinkConn::Mode::RSRP;
    }
    else if (argAs == "cre")
    {
//        UINFO("CRE");
        astype = UplinkConn::Mode::CRE;
    }
    else if (argAs == "pl")
    {
//        UINFO("PL");
        astype = UplinkConn::Mode::PL;
    }
    else
    {
        MSG_ASSERT(false, "Invalid access selection argument");
    }

    gnsm::System net;
    {//==|----> define logging levels
        //        LOG_SET_ALL_LEVEL(LogLevel::ALL)
                LOG_SET_LEVEL("PrintUsers", LogLevel::INFO);
        //        LOG_SET_LEVEL("UplinkEffectiveSinr", LogLevel::INFO);
        //        LOG_SET_LEVEL("RxPower", LogLevel::INFO)
        //        LOG_SET_LEVEL("UlClosestConn", LogLevel::INFO);
        //        LOG_SET_LEVEL("UplinkPowCl", LogLevel::INFO);
        //        LOG_SET_LEVEL("UplinkEffectiveSinr", LogLevel::INFO);
        //        LOG_SET_LEVEL("UplinkPowerLP", LogLevel::INFO);
    }
    {//==|----> create network types
        net.Type<User, UserConf>("USER");
        net.Type<LteUe, LteUeConf>("LTE_UE", USERS_CONF);
        //            net.Type<LteUe, LteUeConf>("LTE_UE", USERS_CONF);
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
    }
    {//==|----> aggregate type
        net.Aggregate("ENBS",{"MACRO", "PICO"});
    }
    {//==|----> locate the eNBs only at start
        net.Initializer<EnbHexLocator>({"MACRO"}, units::m(0), units::m(500), 2);
        net.Initializer<EnbRandLocator>({"PICO"}, units::m(-600), units::m(600));
    }
    {//==|----> set services pattern
        //        net.Initializer<ServiceAlwaysOn>({"USER::*::GENERIC_SERVICE"});
    }
    {//==|----> set valid state at the begining of the snapshot
        net.Action<UserCallUp>({"USER"});
        net.Action<EnbCallUp>({"ENBS"});
    }
    {//==|----> locate the users
        net.Action<RandomAnulusLocator>({"USER"}, units::m(750)); //, 1000);
    }
    {//==|----> scan environment
        net.Action<ParallelLteScan>({"USER", "ENBS"}, AntennaType_e::HV, PropType_e::LOS);
    }
    {//==|----> access selection
        net.Action<UplinkConn>({"USER"}, astype);
    }
    {//==|----> calculate transmission power
        switch (pctype)
        {
        case PrintUsers::UlcpType::OL:
            net.Action<UplinkPowSimple>({"USER"}, UplinkPowSimple::PcMode::OlPc);
            break;
        case PrintUsers::UlcpType::NC:
            net.Action<UplinkPowSimple>({"USER"}, UplinkPowSimple::PcMode::NoPc);
            break;
        case PrintUsers::UlcpType::CL:
            net.Action<UplinkPowCl>({"USER"});
            break;
        case PrintUsers::UlcpType::LP:
        default:
            net.Action<UplinkPowerLP>({"USER"});
            break;
        }
    }
    {//==|----> Effective SINR
        net.Action<UplinkEffectiveSinr>({"USER"});
        //        net.Action<PrintUsers>({"USER", "PICO"}, PrintUsers::PrintType::CONSOLE);
    }
    {//==|----> get results
        auto asTypeStr = (astype == UplinkConn::Mode::CRE ? "CRE" :
                astype == UplinkConn::Mode::RSRP ? "RSRP" : "PL");
//        net.Action<PrintUsers>({"USER", "PICO"}, PrintUsers::PrintType::EFF_SINR, pctype, asTypeStr);
//        net.Action<PrintUsers>({"USER", "PICO"}, PrintUsers::PrintType::UL_TXPOWER, pctype, asTypeStr);
        
        net.Action<PrintUsers>({"USER", "PICO"}, PrintUsers::PrintType::EFF_SINR, pctype, asTypeStr);
        net.Action<PrintUsers>({"USER", "PICO"}, PrintUsers::PrintType::UL_TXPOWER, pctype, asTypeStr);
    }
    auto start = Now();
    net.Run();
//    UINFO(Time2ms(Now() - start), " ms");
    return 0;
}
