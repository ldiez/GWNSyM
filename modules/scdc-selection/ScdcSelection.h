#ifndef SCDCSELECTION_H
#define SCDCSELECTION_H

#include "User.h"
#include "PythonSolver.h"
#include "CreSelection.h"
#include "ScdcUtils.h"

/**
 * \description Perform resource allocation of users and 
 * eNBs by solving the SCDC optimization problem (using GPKIT).
 * It performs only the down-link resource allocation
 */
class ScdcSelection
{
public:
    /**
     * \brief Ctor
     * \param modPath --> Path to the python module
     * \param load --> Load of the interfering cells. This load factor (G-factor) will be applied
     * \param 
     * to the cells passed around within the interfering cells set. Besides, those cell will never
     * be a connection alternative
     */
    ScdcSelection(std::string&& modPath, double backgroundLoad, double interLoad, units::dB cre);
    void operator()(gnsm::Vec_t<User>, gnsm::Vec_t<LteCell> interCells = gnsm::Vec_t<LteCell>());

    std::vector<double> GetSolution(gnsm::Vec_t<User>,
                                    gnsm::Vec_t<LteCell> interCells = gnsm::Vec_t<LteCell>());

    /**
     * \brief Perform connections between to the users and cells
     * \param --> Users vector
     * \return --
     */
    void Connect(void);

private:
    PythonSolver m_solver;
    LocalTranslator m_lt;
    TranslatedVec_t m_tempInfo;
    std::vector<double> m_results;

    /**
     * \brief Call the solver
     * \return --
     */
    void Solve(void);
    void PrintTempInfo(void) const;
    void Check(void) const;
};

#endif /* SCDCSELECTION_H */