#ifndef TWOMINIMA_H
#define	TWOMINIMA_H

#include <random>
#include <queue>

#include "../SaSolution.h"
#include "../Perturbation.h"

namespace SA
{
/**
 * \ingroup SA
 * \brief Example function with two minima for simulated annealing
 */
class TwoMinima : public SaSolution
{
public:
    TwoMinima ( );
    ~TwoMinima ( );
private:
    
    double m_x;
    
    double m_auxX;
    
    std::default_random_engine m_gen;
    
    const std::uint32_t m_queuesSize = 1000000;
    std::queue<double> m_xArray;
    std::queue<double> m_yArray;
    std::queue<double> m_errorArray;
    
    void InitQueues ( void );

    /**
     * \brief From SaSolution
     */
    virtual void DoInitialize ( void );
    virtual void DoTryPerturb ( double temp, double initTemp, Perturbation& pert );
    virtual void DoApprovePerturb ( void );
    virtual double ComputeError ( void );
    virtual double ComputeAuxError ( void );
    virtual void DoPlotResults ( void );

};

} // namespace 
#endif	/* TWOMINIMA_H */

