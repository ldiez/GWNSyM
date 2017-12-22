#ifndef SOLVERSCOMPARER_H
#define SOLVERSCOMPARER_H

#include "SaSelection.h"
#include "ScdcSelection.h"
#include "Trace.h"


/**
 * \brief This class uses both SA-SCDC and GP-SCDC solvers to compare the results.
 * It creates traces to see the difference between the solvers:
 * 
 */
class SolversComparer
{
public:
    /**
     * \breif Ctor
     * \param resPath --> Path for the results file. It creates a folder in the path called 
     * 
     * solvers-comparer:
     * 
     * 
     * \param modPath --> Path of the python modules (for the GP solver)
     * \param backgrounLoad --> Background load of the serving cells
     * \param interLoad --> Load of the interfering cells
     * \param cre --> CRE bias for th pico-cells
     */
    SolversComparer(std::string&& resPath, std::string&& modPath, double backgrounLoad, 
                    double interLoad, units::dB cre);
    ~SolversComparer();
    
    void operator() (gnsm::Vec_t<User> users, gnsm::Vec_t<LteCell> interCells);
private:
    
    void StoreTrace (double gp, double sa);
    void DumpTraces( void );
    
    ScdcSelection m_gp;
    SaSelection m_sa;
    
    std::vector<double> m_assignSa;
    std::vector<double> m_assignGp;
    std::vector<double> m_diff;
    Trace m_trace;
    
    static const std::string FileName_s;
};

#endif /* SOLVERSCOMPARER_H */