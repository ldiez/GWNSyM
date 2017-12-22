#include <cstdlib>
#include <chrono>

// Common headers
#include "System.h"
#include "Configurer.h"

// entities
#include "lte-ae/LteEnb.h"
#include "Cluster/LteCluster.h"
#include "User.h"

#include "ParallelLteScan.h"
#include "PosRaster-2d.h"
#include "SimpleSelection.h"
#include "Cluster/LteClusterLocator.h"
#include "CallUp.h"
#include "ServiceAlwaysOn.h"
#include "ScdcSelection.h"
#include "SquareRandomLocator.h"
#include "PrintCells.h"
#include "LteScanMetric.h"

#include "SolversComparer.h"
#include "EnbLoadTrace.h"
#include "LteUsersConnection.h"
#include "SimpleUplink.h"

// configuration paths
#define GLOB_CONF "./config-files/SCDC/GLOB_CONF.cfg"
#define ENBS_CONF "./config-files/SCDC/ENBS.xml"
#define USERS_CONF "./config-files/SCDC/USER.xml"

/**
 * \description Scenario to check the Single Connection Demand Constraint problem
 * 
 * This scenario is an LTE-only scenario.
 * The users have a traffic demand.
 * The access network consists of a set of clusters.
 * Each cluster has one macro eNB and a number of small cells
 *      - Macro eNBs have 3 sectors.
 *      - Small cells are omni-directional
 */
int
main(int argc, char** argv)
{
    gnsm::System net_;

    LOG_SET_LEVEL("LteUe", LogLevel::INFO);
    LOG_SET_LEVEL("SimpleUplink", LogLevel::ALL);

    net_.AddType<User, UserConf>("USER");
    net_.AddType<LteUe, LteUeConf>("LTE_UE", USERS_CONF);

    net_.AddType<LteCell, LteCellConf>("MACRO_CELL", EnbType::MACRO, ENBS_CONF);
    net_.AddType<LteEnb, LteEnbConf>("MACRO", EnbType::MACRO, ENBS_CONF);
    net_.AddType<LteCell, LteCellConf>("PICO_CELL", EnbType::PICO, ENBS_CONF);
    net_.AddType<LteEnb, LteEnbConf>("PICO", EnbType::PICO, ENBS_CONF);
    net_.AddType<LteCluster, LteClusterConf>("CLUSTER_SERV", ENBS_CONF, "CLUSTER_SERV");
    net_.AddType<LteCluster, LteClusterConf>("CLUSTER_INTER", ENBS_CONF, "CLUSTER_INTER");
    net_.AddType<Service, ServConf>("GENERIC_SERVICE", ServType::GENERIC, USERS_CONF);

    // Global deployment
    net_.AddGlobalConfigFinal("CLUSTER_SERV",{"CLUSTER_SERV", "NUMBER"});
    net_.AddGlobalConfigFinal("CLUSTER_INTER",{"CLUSTER_INTER", "NUMBER"});
    net_.AddGlobalConfigFinal("USER",{"USER", "NUMBER"});
    net_.SetConfig<gnsm::Configurer>(GLOB_CONF);
    net_.Deploy();

    net_.Aggregate("SERVING_ENBS",{"CLUSTER_SERV::*::MACRO", "CLUSTER_SERV::*::PICO"});
    net_.Aggregate("ALL_ENBS",{"CLUSTER_SERV::*::MACRO", "CLUSTER_SERV::*::PICO",
                   "CLUSTER_INTER::*::MACRO", "CLUSTER_INTER::*::PICO"});
    net_.Aggregate("ALL_CLUSTERS",{"CLUSTER_SERV", "CLUSTER_INTER"});
    net_.Aggregate("INTER_CELLS",{"CLUSTER_INTER::*::MACRO::*::MACRO_CELL",
                   "CLUSTER_INTER::*::PICO::*::PICO_CELL"});

    net_.Initializer<LteClusterLocator>({"ALL_CLUSTERS"}, units::m(10000.0), units::m(700),
            3, units::m(350.0), units::m(200.0));

    // set the user in the middle
    net_.Action<SquareRandomLocator>({"USER"}, units::m(4650.0), units::m(5350.0));
    net_.Action<ServiceAlwaysOn>({"USER::*::GENERIC_SERVICE"});

    // Call-up to be ready for a new snapshot
    net_.Action<UserCallUp>({"USER"});
    net_.Action<EnbCallUp>({"ALL_ENBS"});

    net_.Action<ParallelLteScan>({"USER", "ALL_ENBS"}, AntennaType_e::HONLY, PropType_e::FULL);
    net_.Action<SimpleUplink>({"USER"}); 
//    net_.Action<SaSelection>({"USER", "INTER_CELLS"}, 0.0, 0.0, units::dB(0.0));
//    net_.Action<EnbLoadTrace>({"SERVING_ENBS"}, "./RESULTS/SCDC");
//    net_.Action<LteUsersConnection>({"USER"}, "./RESULTS/SCDC");

    auto start_ = std::chrono::steady_clock::now();
    net_.Run();
    std::cout << std::chrono::duration_cast <std::chrono::milliseconds>
            (std::chrono::steady_clock::now() - start_).count() << "ms" << std::endl;
    return 0;
}