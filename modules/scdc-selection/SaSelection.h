#ifndef SASELECTION_H
#define SASELECTION_H


#include "User.h"
#include "lte-ae/LteCell.h"

#include "SaUtility.h"
#include "SaScdc.h"

/**
 * \description Wrapper to perform the selection based on CRE and the resource allocation
 * by using simulated annealing
 * 
 * It uses: 
 *  - derived SaSolution to perform the annealing
 *  -a utility class to compute the cost: this class stores the mutual interference values 
 *  -Info carrier to carry the information regarding the assignment 
 */
class SaSelection
{
public:
    /**
     * \brief Ctor
     * \param load --> G-factor used for the interfering cells
     * \param cre --> CRE bias to apply to \PICO cells
     */
    SaSelection(double backgroundLoad, double interLoad, units::dB cre);
    ~SaSelection();


    void operator()(gnsm::Vec_t<User>, gnsm::Vec_t<LteCell> interCells = gnsm::Vec_t<LteCell>());

    std::vector<double> GetSolution(gnsm::Vec_t<User>,
                                    gnsm::Vec_t<LteCell> interCells = gnsm::Vec_t<LteCell>());

    void Connect(void);

private:
    
    void ConnectUser (TranslatedInfo& ti, std::uint32_t index);

    void Setup(gnsm::Vec_t<User>&, gnsm::Vec_t<LteCell>&);
    void Solve(void);



    SaUtility m_utility;
    SaScdc m_sa;
    LocalTranslator m_lt;

    InfoCarrier m_ic;
    TranslatedVec_t m_tempInfo;
    std::vector<double> m_results;
};

#endif /* SASELECTION_H */

