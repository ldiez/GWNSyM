#ifndef SASCDC_H
#define SASCDC_H

#include <functional>
#include "SaSolution.h"
#include "InfoCarrier.h"

/**
 * \brief This class performs the simulated annealing of the resources allocation over all the LTE
 * network. The network must be abstracted.
 * 
 * For all: 
 *  - Initial value
 * 
 * For each element it is given:
 *  - Target demand
 *  - Current demand 
 *  - Seff
 * 
 * The error calculation must be provided by a callback
 */
class SaScdc final : public SA::SaSolution
{
public:
    using UtilityCaller_t = std::function<double (InfoCarrier&) >;

    SaScdc(UtilityCaller_t uc);
    ~SaScdc();
    
    void SetCaller (UtilityCaller_t uc);
    
    /**
     * \brief Set the current demand information
     * \param ic --> Demand information
     * \return --
     */
    void SetInfo ( InfoCarrier& ic );


    /**
     * \brief Perform annealing 
     * \param users <--> Current local configuration (selection is established). 
     * \param hold --> Indicate if containers binded to external info are to be flushed. 
     * By default it is done so after the annealing, otherwise \Reset is to be called
     * \return --
     *
     */
    void Anneal(void);

private:

    /**
     * \brief Launch simulated annealing process
     */
    void LaunchAnnealing(void);

    /**
     * \brief Load the best found solution, it is called only at the end
     * \return --
     */
    void LoadBest(void);
    
    /**
     * \brief Update the best assignment only if the m_auxError improves the current one
     * \return --
     */
    void UpdateBest (void);
    
    /**
     * \brief Inherited from SA::SaSolution
     */
    void DoInitialize(void) final;
    void DoTryPerturb(double temp, double initTemp, SA::Perturbation& pert) final;
    void DoApprovePerturb(void) final;
    double ComputeError(void) final;
    double ComputeAuxError(void) final;
    void DoPlotResults(void) final;
    
    /**
     * \brief Calculate the assignment when there is not mutual interference
     * \param i --> Index of the element
     * \return <-- Assignment
     */
    double SimpleAssignment (std::uint32_t i) const;

    // utility calculation
    UtilityCaller_t m_utilityCaller;
    InfoCarrier* m_ic;
    
    // evolution of the best error
    std::vector<double> m_evolution;
    std::vector<double> m_auxErrVec;

    // this is the overall best assignment
    std::vector<double> m_bestAssigment;
    
    // this is the current assignment
    std::vector<double> m_currAssigment;
    
    double m_bestError;
    
    

};

#endif /* SASCDC_H */