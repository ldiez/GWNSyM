#ifndef ENBHEXLOCATOR_H
#define ENBHEXLOCATOR_H

#include "CoreUtils.h"
#include "Distance.h"

class LteEnb;
class EnbHexLocator {
public:
    EnbHexLocator(units::m side, units::m isd, std::uint32_t rings);
    
    void operator() (gnsm::Vec_t<LteEnb> es);
private:
    void HexPosition(void);
    const units::m m_side;
    const units::m m_isd;
    const std::uint32_t m_rings;
    std::uint32_t m_ctr;
    std::vector<std::pair<double, double>> m_coord;

};



#endif /* ENBHEXLOCATOR_H */

