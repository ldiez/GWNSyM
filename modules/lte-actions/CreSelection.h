#ifndef CRESELECTION_H
#define CRESELECTION_H

#include <map>
#include "lte-user/LteUe.h"

/**
 * \description Perform the eNB selection by applying CRE factors
 * @param picoCre
 * @param microCre
 * @param macroCre
 */
class CreSelection
{
public:
    /**
     * \brief By default the CRE factors are 0
     * \param picoCre --> CRE factor for pico cells
     * \param microCre --> CRE factor for micro cells
     * \param macroCre --> CRE factor for macro cells
     */
    CreSelection(units::dB picoCre = units::dB(0.0), units::dB microCre = units::dB(0.0),
                 units::dB macroCre = units::dB(0.0));
    
    /**
     * \brief Perform the selection
     * \param sensed --> List of cells scanned
     * \return <-- Pair with the selected cell and the RSRP value 
     */
    std::pair<gnsm::Ptr_t<LteCell>, Power> Select(LteUe::SensedValues_t const& sensed);
private:
    const std::map<EnbType, units::dB> m_creMap;
};

#endif /* CRESELECTION_H */

