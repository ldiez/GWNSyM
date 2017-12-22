#ifndef COOLINGSCHEDULE_H
#define	COOLINGSCHEDULE_H

namespace SA
{

/**
 * \ingroup SA
 * \breif This class implements the cooling schedule for simulated annealing algorithms. 
 * It implements:
 * - Linear schedule
 * - Logarithmic schedule
 * - Geometric schedule 
 * - Exponential schedule
 * Each schedule has different parameters and they are required to be explicitely set by the 
 * library user to use the schedule.
 */

enum class CoolingType_e : std::uint8_t
{
    LINEAR,
    LOG,
    GEOM,
    EXP
};

class CoolingSchedule
{
public:
    /**
     * \brief Ctor. Initial temperature is set at construction, by default final temperature is 0
     * \param initTemp --> Initial temperature
     */
    CoolingSchedule ( double initTemp );
    ~CoolingSchedule ( );
    
    /**
     * \brief Set cooling type
     * \param type --> Cooling type (LOG, EXP, GEOM, LINEAR)
     * \return --
     */
    void SetCoolingType ( CoolingType_e type );
    
    /**
     * \brief Set cooling parameter for shcedules logarithmic, geometric and exponential
     * \param alpha --> Cooling parameter
     * \return --
     */
    void SetCoolingParameter ( double alpha ); 
    
    /**
     * \brief Set size of the search space
     * \param ss --> Size of the search space
     * \return --
     */
    void SetSerachSpace ( std::uint32_t ss ); 
    
    /**
     * \brief Set final temperature
     * \param finalTemp --> Final temperature
     * \return --
      */
    void SetFinalTemp ( double finalTemp ); 
    
    /**
     * \brief Set number of temperatures to try
     * \param numTemps --> Number of temperatures
     * \return --
     */
    void SetNumTemps ( std::uint32_t numTemps );

    /**
     * \berief Get temperature value in the k-th iteration
     * \param iter --> Iteration number
     * \return <-- Value
     */
    double GetValue ( std::uint32_t iter );

private:
    /**
     * \brief Implementation of the logarithmic cooling
     * \param iter --> Current iteration
     * \return <-- Value
     */
    double LogFunc ( std::uint32_t iter );

    /**
     * \brief Implementation of the geometric cooling
     * \param iter --> Current iteration
     * \return <-- Value
     */
    double GeomFunc ( std::uint32_t iter );

    /**
     * \brief Implementation of the exponential cooling
     * \param iter --> Current iteration
     * \return <-- Value
     */
    double ExpFunc ( std::uint32_t iter );

    /**
     * \brief Implementation of linear cooling
     * \param iter --> Current iteration
     * \return <-- Value
     */
    double LinearFunc ( std::uint32_t iter );

    CoolingType_e m_coolingType = CoolingType_e::EXP;
    double m_initTemp;
    double m_finalTemp = 0.0;
    std::uint32_t m_numTemps = 0;
    std::uint32_t m_searchSpace = 0;
    double m_alpha = 0.0;
};
} // namespace SA
#endif	/* COOLINGSCHEDULE_H */