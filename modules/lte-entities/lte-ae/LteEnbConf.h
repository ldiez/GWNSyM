#ifndef LTEENBCONF_H
#define LTEENBCONF_H

#include "CoreUtils.h"
#include "Bandwidth.h"
#include "../LteCommon.h"

class LteEnbConf
{
public:
    LteEnbConf(EnbType enbType, std::string&& path);

    gnsm::InnerItemsCounterList_t ReadInnerConf(void) const;

    /**
     * \breif Provide sectorization angle (typically 120º)
     * \return <-- Angle
     */
    double GetSectorizationAngle(void) const;
    units::GHz GetFrequency(void) const;
    EnbType GetType(void) const;

    std::string GetCellSetName(void) const;

private:
    double m_angle;
    std::uint32_t m_numCells;
    const EnbType m_enbType;
    units::GHz m_freq;
    std::string m_cellSetName;
};

#endif /* LTEENBCONF_H */

