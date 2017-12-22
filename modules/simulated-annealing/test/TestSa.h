#ifndef TESTSA_H
#define	TESTSA_H

#include <cmath>

#include "TestSaPerturbation.h"
#include "SimulatedAnnealing.h"
#include "SaNumEq.h"
#include "TwoMinima.h"
#include "gnuplot_i.h"

namespace SA
{

inline void TestSa ( void )
{    
    LOG_SET_LEVEL ("SimulatedAnnealing", LogLevel::DEBUG);
    LOG_SET_LEVEL ("SaTwoMinima", LogLevel::DEBUG);
    LOG_SET_LEVEL ("SaCoolingSchedule", LogLevel::DEBUG);
    SA::SimulatedAnnealing sa_(100.0);
    sa_.SetParam("INIT_TEMP", 100.0)
            .SetParam("FINAL_TEMP", 0.0)
            .SetParam("NUM_TEMPS", 10000)
            .SetParam("NUM_ITER", 10)
            .SetParam("NUM_CYCLES", 1)
            .SetParam("GOAL", 0)
            .SetParam("SEARCH_SPACE", 1)
            .SetParam("COOLING_PARAM", .9)
            .SetParam("COOLING_TYPE", "EXP")
            .SetParam("PERT_SPREAD_FACTOR", 10.0)
            .SetParam("PERT_TYPE", "CAUCHY")
            .SetParam("TIMES_TRIALS", 100);
    
    TwoMinima sol_;
    sa_.Start(sol_);
    sol_.PlotResults();
    
}

}// namespace SA

#endif	/* TESTSA_H */

