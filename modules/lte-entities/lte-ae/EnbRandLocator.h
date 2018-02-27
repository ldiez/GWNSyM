#ifndef ENBRANDLOCATOR_H
#define ENBRANDLOCATOR_H

#include "CoreUtils.h"
#include "Distance.h"
#include <random>

class LteEnb;

class EnbRandLocator {
public:
    EnbRandLocator(units::m side);
    EnbRandLocator(units::m min, units::m max);
    EnbRandLocator(units::m xmin, units::m xmax, units::m ymin, units::m ymax);
    void operator()(gnsm::Ptr_t<LteEnb> e);

private:
    std::default_random_engine m_re;
    std::uniform_real_distribution<double> m_xDist;
    std::uniform_real_distribution<double> m_yDist;

};

#endif /* ENBRANDLOCATOR_H */

