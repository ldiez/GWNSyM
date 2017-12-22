#ifndef SIMULATEDANNEALING_H
#define	SIMULATEDANNEALING_H

#include "SaSolution.h"
#include "CoolingSchedule.h"
#include "Perturbation.h"
#include "AcceptanceCriterion.h"
#include <map>
namespace SA
{

enum class SaParamEnum : std::uint8_t
{
    INIT_TEMP, // initial temperature
    FINAL_TEMP, // final temperature
    NUM_TEMPS, // number of temperatures
    NUM_ITER, 
    NUM_CYCLES,
    TIMES_TRIALS,
    GOAL,
    SEARCH_SPACE,
    COOLING_PARAM,
    COOLING_TYPE,
    PERT_SPREAD_FACTOR,
    PERT_TYPE
};

static const std::map<std::string, SaParamEnum> SaParamMap{
    {"INIT_TEMP", SaParamEnum::INIT_TEMP },
    {"FINAL_TEMP", SaParamEnum::FINAL_TEMP },
    {"NUM_TEMPS", SaParamEnum::NUM_TEMPS },
    {"NUM_ITER", SaParamEnum::NUM_ITER },
    {"NUM_CYCLES", SaParamEnum::NUM_CYCLES },
    {"TIMES_TRIALS", SaParamEnum::TIMES_TRIALS},
    {"GOAL", SaParamEnum::GOAL },
    {"SEARCH_SPACE", SaParamEnum::SEARCH_SPACE },
    {"COOLING_PARAM", SaParamEnum::COOLING_PARAM },
    {"COOLING_TYPE", SaParamEnum::COOLING_TYPE },
    {"PERT_SPREAD_FACTOR", SaParamEnum::PERT_SPREAD_FACTOR },
    {"PERT_TYPE", SaParamEnum::PERT_TYPE }
};

/**
 * \ingroup SA
 * \breif This class implements the whole simulated annealing algorithm in two main loops:
 * - Thermal equilibrium loop (inner): that simulates the achievement of thermal equilibrium at a 
 * given temperature
 * - Annealing loop (outer): that performs the cooling process in which the temperature decreases 
 * until certain convergence criterion
 */
class SimulatedAnnealing
{
public:
    SimulatedAnnealing ( ) = delete;
    /**
     * \brief Constructor, all the configuration parameters are to be set
     * \param initTemp --> Initial temperature
     * \param finalTemp --> Final temperature
     * \param numTemps --> Number of temperatures to check
     * \param numIter --> Number of iterations
     * \param cycles --> Number of cycles
     * \param k --> Initial k-value for the acceptance criterion, optional
     */
    SimulatedAnnealing ( double initTemp );
    ~SimulatedAnnealing ( ) = default;

    /**
     * \brief Set optimization parameter for values
     * \param paramName --> Name of the parameter
     * \param value --> Parameter value. The type is double, but in will be converted according 
     * to the expected type: integer, unsigned integer or floating point value
     * \return <--> Reference to itself
     */
    SimulatedAnnealing& SetParam ( std::string paramName, double value );

    /**
     * \brief Set optimization parameter by string. It is implemented because some parameters 
     * (e.g. cooling schedule type) cannot be set by value
     * \param paramName --> Name of the parameter
     * \param type --> Value by string
     * \return <--> Reference to itself
     */
    SimulatedAnnealing& SetParam ( std::string paramName, std::string type );

    /**
     * \brief Start the annealing process 
     * \param sol <--> Actual solution
     * \return <-- solution
     */
    double Start ( SaSolution& sol );

    /**
     * \brief Get a new temperature. It will call the cooling class.
     * \param index --> Current iteration
     * \return <-- New temperature
     */
    double GetTemperature ( std::uint32_t iter );

    /**
     * \brief Check whether or not a new solution is to be accepted. 
     * \param temp --> Current temperature
     * \param deltaError --> Current error increment
     * \return <-- True if the new solution is accepted, false otherwise.
     */
    bool IsAcceptable ( double temp, double deltaError );

    /**
     * \brief Initiate a new annealing process
     * \param sol <--> Current solution class 
     * \param goal --> Target result
     * \return <-- The 
     */
    double Anneal ( SaSolution& sol );
    
    /**
     * \brief Perform the thermal equilibrium loop.
     * \param sol <--> Current Solution class
     * \param iter --> Current iteration
     * \return <-- A boolean that indicates if the cooling process is to continue. If a number of 
     * trials have been performed without getting a new acceptable solution, the cooling process 
     * is stopped.
     */
    bool ThermalEquilibrium( SaSolution& sol, std::uint32_t iter );
    

    /**
     * \brief Estimate value of the \k parameter for the acceptance criterion.
     * k = T0 * ln(0.8)/stdDev(error)
     * Where the variance is estimated as: var = 
     * \param sol <--> Actual solution
     * \param n --> 
     * \return <-- New value of the K parameter.
     */
    double EstimateK ( SaSolution& sol, std::uint32_t n );

    double m_initTemp = 0.0;
    double m_finalTemp = 0.0;
    std::uint32_t m_numTemp = 0;
    std::uint32_t m_numIter = 0;
    std::uint32_t m_cycles = 0;
    double m_goal = 0.0;
    std::uint32_t m_timesTrials = 10;

    // Cooling configuration
    double m_K = 0.0;
    std::uint32_t m_searchSpace = 0;
    double m_coolingParam = 0.0;
    CoolingType_e m_coolingType;
    
    // Perturbation configuration
    double m_pertSpreadFactor = 0;
    PertType_e m_pertType;


    /**
     * Classes to implement different models of cooling, acceptance and perturbation
     */
    CoolingSchedule m_cool;
    AcceptanceCriterion m_accept;
    Perturbation m_pert;
};

}; // namespace SA
#endif	/* SIMULATEDANNEALING_H */