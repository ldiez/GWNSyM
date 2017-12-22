#include <cstdlib>
#include <chrono>

// utils
#include "System.h"
#include "Configurer.h"

// entities
#include "lte-ae/LteEnb.h"
#include "Cluster/LteCluster.h"
#include "User.h"

//actions 
#include "ParallelLteScan.h"

// updaters/claspers
//#include "prop-test/PosRaster-2d.h"
//#include "prop-test/PropPrintResults.h"
//#include "prop-test/FlushUsersInfo.h"
#include "SimpleSelection.h"
#include "Cluster/LteClusterLocator.h"
#include "PrintCells.h"


// configuration paths
#define GLOB_CONF "./config-files/GLOB_CONF.cfg"
#define ENBS_CONF "./config-files/ENBS.xml"
#define USERS_CONF "./config-files/USER.xml"

int
main(int argc, char** argv)
{

    /**
     * \description
     * This scenario performs an hexagonal deployment of macro base stations
     * along with some pico cells.
     * Afterwards, a basic access policy will be performed.
     * 
     */
//    gnsm::System net_;
//    //    LOG_SET_LEVEL("LteEnbConf", LogLevel::INFO);
//    //    LOG_SET_LEVEL("LteCellConf", LogLevel::INFO);
//    //    LOG_SET_LEVEL("LteUeConf", LogLevel::INFO);
//    //    LOG_SET_LEVEL("LteUe", LogLevel::INFO);
//    //    LOG_SET_LEVEL("RxPower", LogLevel::INFO);
//    //    LOG_SET_LEVEL("Antenna", LogLevel::INFO);
//    //    LOG_SET_LEVEL("Sinr", LogLevel::INFO);
//    //    LOG_SET_LEVEL("PropTestResults", LogLevel::ALL);
//    LOG_SET_LEVEL("LteClusterLocator", LogLevel::INFO);
//    //    LOG_SET_LEVEL("LteEnb", LogLevel::INFO);
//
//    net_.AddType<User, UserConf>("USER");
//    net_.AddType<LteUe, LteUeConf>("LTE_UE", USERS_CONF);
//
//    net_.AddType<LteCell, LteCellConf>("MACRO_CELL", EnbType::MACRO, ENBS_CONF);
//    net_.AddType<LteEnb, LteEnbConf>("MACRO", EnbType::MACRO, ENBS_CONF);
//    net_.AddType<LteCell, LteCellConf>("PICO_CELL", EnbType::PICO, ENBS_CONF);
//    net_.AddType<LteEnb, LteEnbConf>("PICO", EnbType::PICO, ENBS_CONF);
//    net_.AddType<LteCluster, LteClusterConf>("CLUSTER", ENBS_CONF);
//
//
//    net_.AddGlobalConfigFinal("CLUSTER",{"CLUSTER", "NUMBER"});
//    net_.AddGlobalConfigFinal("USER",{"USER", "NUMBER"});
//    net_.SetConfig<gnsm::Configurer>(GLOB_CONF);
//    net_.Deploy();
//    net_.PrintInstances(gnsm::ts::NAME::FULL);
//
//    net_.ApplyOnceUpdater<LteClusterLocator>("CLUSTER", units::m(1000.0), units::m(450), 
//            units::m(400.0), units::m(300.0));
//    //    net_.ApplyUpdater<PosRaster_2d>("USER", true, 5.0, 2000.0);
//    //        net_.ApplyAction<ParallelLteScan>({"USER", "CLUSTER::*::MACRO"});
//    //    net_.ApplyAction<SimpleSelection>({"USER"}, 1, 1.0);
//    //        net_.ApplyAction<PropPrintResults>({"USER"});
//    //    net_.ApplyAction<FlushUsersInfo>({"USER"});
//    //
//    net_.ApplyAction<PrintCells>({"CLUSTER::*::MACRO", "CLUSTER::*::PICO"});
//    net_.Run();
//
//    //    auto start_ = std::chrono::steady_clock::now();
//    //    std::cout << std::chrono::duration_cast <std::chrono::milliseconds>
//    //            ( std::chrono::steady_clock::now() - start_ ).count() 
//    // << "ms" << std::endl;
    return 0;
}