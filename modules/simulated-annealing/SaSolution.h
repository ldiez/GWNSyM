#ifndef SASOLUTION_H
#define	SASOLUTION_H

#include "Perturbation.h"

namespace SA
{
/**
 * \ingroup SA
 * \brief Abstract class for simulated annealing problems. Specializations of these class will 
 * be implemented to interface with the real problem.
 */
class SaSolution
{
public:
    SaSolution ( );
    virtual ~SaSolution ( ) = default;

    /**
     * \brief Initializes the current solution
     * \return <-- Error induced by the inizialitation
     */
    double Initialize ( void );

    /**
     * \brief Return the new error value of a perturbation
     * \param temp --> Current temperature 
     * \param initTemp --> Initial temperature
     * \param pert --> Reference to perturbation class
     * \return <-- Cost value of the new perturbation 
     */
    double TryPerturb ( double temp, double initTemp, SA::Perturbation& pert );
    
    /**
     * \brief Approve the last perturbation
     * \return --
     */
    void ApprovePerturb ( void );

    /**
     * \brief Get current error
     * \return <-- Error value
     */
    double GetError ( void );
    
    /**
     * \brief Plot results
     * \return --
     */
    void PlotResults ( void );
    
private:

    /**
     * \breif Implement initialization
     * \return --
     */
    virtual void DoInitialize ( void ) = 0;

    /**
     * \brief Implement perturbation trial
     * \param temp --> Current temperature 
     * \param initTemp --> Initial temperature
     * \param pert --> Reference to perturbation class
     * \return --
     */
    virtual void DoTryPerturb ( double temp, double initTemp, Perturbation& pert ) = 0;
    
    /**
     * \brief Implement perturbation approval
     * \return --
     */
    virtual void DoApprovePerturb ( void ) = 0;

    /**
     * \brief Implement error computation for the current values
     * \return <-- Error value
     */
    virtual double ComputeError ( void ) = 0;
    
    /**
     * \brief Implement error computation for trial values
     * \return <-- Error value
     */
    virtual double ComputeAuxError ( void ) = 0;
    
    /**
     * \brief Implement results plotting
     * \return --
     */
    virtual void DoPlotResults ( void ) = 0;

protected:    
    double m_error;
    double m_auxError;

};

} // namespace SA
#endif	/* SOLUTION_H */