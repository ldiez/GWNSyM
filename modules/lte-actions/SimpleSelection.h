#ifndef SIMPLESELECTION_H
#define SIMPLESELECTION_H

#include "User.h"
#include "Sinr.h"

/**
 * \brief This class performs the access selection of the \n cells with higher 
 * SINR. Where \n is indicated in the constructor
 * The SINR is calculated assuming a constant load \l in all the cells. The load 
 * parameter \l is passed in the constructor
 * If uplink traffic exists, it is also sent to the same cells.
 * It assumes that the bandwith if the resources is 180KHz
 */



class SimpleSelection
{
public:

    /**
     * \brief Ctor
     * \param maxCells --> Max. number of cells to connect
     * \param load --> Average load to calculate the SINR
     */
    SimpleSelection(std::uint32_t maxCells, double load);

    void operator()(gnsm::Ptr_t<User> user);
private:

    struct TempInfo
    {
        gnsm::Vec_t<LteCell> m_selectedCells;
        Sinr m_sinr;
        Traffic m_dl;
        double m_rbDl;
    };
    
    SimpleSelection() = delete;
    TempInfo m_temp;
    void CalculateResources(void);
    void CalculateSinr(gnsm::Ptr_t<LteUe>);

    void DoSinr(LteUe::SensedValues_t serving, LteUe::SensedValues_t interfering);

    const std::uint32_t m_maxCells;
    const double m_load;
};

#endif /* SIMPLESELECTION_H */

