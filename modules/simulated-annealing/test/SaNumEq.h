#ifndef SANUMEQ_H
#define	SANUMEQ_H

#include <random>
#include <queue>

#include "../SaSolution.h"
#include "../Perturbation.h"

namespace SA
{

/**
 * \ingroup SA
 * \brief Example class for simulated annealing interface
 */
class SaNumEq : public SaSolution
{
public:
    SaNumEq ( );
    ~SaNumEq ( );
private:

    double m_x;
    double m_y;

    double m_auxX;
    double m_auxY;

    std::mt19937 m_gen;
    
    const std::uint32_t m_queuesSize = 1000000;
    std::queue<double> m_xArray;
    std::queue<double> m_yArray;
    std::queue<double> m_errorArray;

    /**
     * \brief From SaSolution
     */
    virtual void DoInitialize ( void );
    virtual void DoTryPerturb ( double temp, double initTemp, Perturbation& pert );
    virtual void DoApprovePerturb ( void );
    virtual double ComputeError ( void );
    virtual double ComputeAuxError ( void );
    virtual void DoPlotResults ( );
};
} // namespace SA
#endif	/* SANUMEQ_H */