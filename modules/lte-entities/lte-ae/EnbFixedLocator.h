#ifndef ENBFIXEDLOCATOR_H
#define ENBFIXEDLOCATOR_H

#include "CoreUtils.h"
#include "Position.h"
#include "Distance.h"

class LteEnb;
class EnbFixedLocator
{
public:
    EnbFixedLocator(std::vector<Position>);
    
    void operator() (gnsm::Vec_t<LteEnb> es);
private:
    const std::vector<Position> m_pos;

};

#endif /* ENBFIXEDLOCATOR_H */

