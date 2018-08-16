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

enum class PcType : std::uint8_t
{
    CL = 0,
    OL,
    LP,
    NOPC
};

int
main(int argc, char** argv)
{
    std::vector<double> alphas{0};
    PcType pctype = PcType::LP;
    PrintUsers::UlcpType pcprint = PrintUsers::UlcpType::LP;
    std::string argPc = argv[1];
    if (argPc == "ol")
    {
        //alphas = {.4, .5, .6, .7, .8, .9, 1};
        alphas = {1};
        UINFO("OL power control");
        pctype = PcType::OL;
        pcprint = PrintUsers::UlcpType::OL;
    }
    else if (argPc == "lp")
    {
        UINFO("LP power control");
        pctype = PcType::LP;
        pcprint = PrintUsers::UlcpType::LP;
    }
    else if (argPc == "nopc")
    {
        UINFO("NOPC power control");
        pctype = PcType::NOPC;
        pcprint = PrintUsers::UlcpType::NC;
    }
    else if (argPc == "cl")
    {
        UINFO("CL power control");
        pctype = PcType::CL;
        pcprint = PrintUsers::UlcpType::CL;
    }

    //    std::vector<double> alphas = {1};
    for (auto alpha : alphas)
    {
        gnsm::System net;
        {//==|----> define logging levels
            //                                    LOG_SET_ALL_LEVEL(LogLevel::ALL)
            LOG_SET_LEVEL("UplinkPowCl", LogLevel::INFO);
            //            LOG_SET_LEVEL("UplinkEffectiveSinr", LogLevel::WARNING);
            //    LOG_SET_LEVEL("RxPower", LogLevel::INFO)
            //        LOG_SET_LEVEL("UlClosestConn", LogLevel::INFO);
            //        LOG_SET_LEVEL("UplinkPowCl", LogLevel::INFO);
            //            LOG_SET_LEVEL("UplinkEffectiveSinr", LogLevel::INFO);
            //            LOG_SET_LEVEL("UplinkPowerLP", LogLevel::INFO);
        }
        {//==|----> create network types
            net.Type<User, UserConf>("USER");
            net.Type<LteUe, LteUeConf>("LTE_UE", USERS_CONF, alpha);
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
            //            net.PrintInstances(gnsm::ts::NAME::FULL);
        }
        {//==|----> aggregate type
            net.Aggregate("ENBS",{"MACRO", "PICO"});
        }
        {//==|----> locate the eNBs only at start
            net.Initializer<EnbHexLocator>({"MACRO"}, units::m(0), units::m(500), 2);
            //            net.Initializer<EnbRandLocator>({"PICO"}, units::m(-600), units::m(600));
            //            std::vector<Position> pos{
            //                {0, 0, 10},
            //                {0, 600, 10}};
            //            net.Initializer<EnbFixedLocator>({"PICO"}, pos);
        }
        {//==|----> set services pattern
            net.Action<ServiceAlwaysOn>({"USER::*::GENERIC_SERVICE"});
        }
        {//==|----> set valid state at the begining of the snapshot
            net.Action<UserCallUp>({"USER"});
            net.Action<EnbCallUp>({"ENBS"});
        }
        {//==|----> locate the users
            //            net.Action<SquareRandomLocator>({"USER"}, units::m(-600), units::m(600));
            net.Action<RandomAnulusLocator>({"USER"}, units::m(750), 1000);
            //            std::vector<Position> pos{
            //                {0, 200, 1.5},
            //                {0, 400, 1.5},
            //                {0, 150, 1.5},
            //                {0, 200, 1.5},
            //                {0, 250, 1.5},
            //                {0, 300, 1.5},
            //                {0, 350, 1.5},
            //                {0, 400, 1.5},
            //                {0, 450, 1.5}};
            //            net.Action<UserFixedLocator>({"USER"}, pos);
        }
        {//==|----> scan environment
            net.Action<ParallelLteScan>({"USER", "ENBS"}, AntennaType_e::HV, PropType_e::FULL);
            //        net.Action<RxPower>({"USER", "ENBS"}, AntennaType_e::HONLY, PropType_e::LOS);
        }
        {//==|----> access selection
            //            net.Action<UplinkConn>({"USER"}, UplinkConn::Mode::PL, units::dB(6.0), units::dB(3.0), units::dB(0.0));
            net.Action<UplinkConn>({"USER"}, UplinkConn::Mode::RSRP);
            //                        net.Action<UlClosestConn>({"USER", "ENBS"});
        }
        {//==|----> calculate transmission power
            //    
            switch (pctype)
            {
            case PcType::OL:
                net.Action<UplinkPowSimple>({"USER"}, UplinkPowSimple::PcMode::OlPc);
                break;
            case PcType::NOPC:
                net.Action<UplinkPowSimple>({"USER"}, UplinkPowSimple::PcMode::NoPc);
                break;
            case PcType::CL:
                net.Action<UplinkPowCl>({"USER"});
                break;
            case PcType::LP:
            default:
                net.Action<UplinkPowerLP>({"USER"});
                break;
            }
        }
        {//==|----> 

            net.Action<UplinkEffectiveSinr>({"USER"});
            //                        net.Action<PrintUsers>({"USER"}, PrintUsers::PrintType::CONSOLE);
            //            net.Action<PrintCells>({"MACRO"}, PrintCells::PrintType::POSITION, "./tests/", "MACRO");
        }
        {//==|----> get results
            net.Action<PrintUsers>({"USER"}, PrintUsers::PrintType::EFF_SINR, pcprint);
            net.Action<PrintUsers>({"USER"}, PrintUsers::PrintType::UL_TXPOWER, pcprint);
            //            net.Action<PrintUsers>({"USER"}, PrintUsers::PrintType::RSRPDIST, PrintUsers::UlcpType::LP);
            //            net.Action<PrintUsers>({"USER"}, PrintUsers::PrintType::EFF_SINR, PrintUsers::UlcpType::CL);
            //            net.Action<PrintUsers>({"USER"}, PrintUsers::PrintType::UL_TXPOWER, PrintUsers::UlcpType::CL);
        }
        auto start = Now();
        net.Run();
        UINFO(Time2ms(Now() - start), " ms");
    }
    return 0;
}