#ifndef RANDOMANULUSLOCATOR_H
#define RANDOMANULUSLOCATOR_H

#include "User.h"
#include "Distance.h"
#include <random> 

class RandomAnulusLocator {
public:
    RandomAnulusLocator(Position center, units::m min, units::m max);
    void operator () ( gnsm::Ptr_t<User> user);
    void SetIteration ( std::uint32_t iter ){};
private:
    std::default_random_engine m_re;
    std::uniform_real_distribution<double> m_range;
    std::uniform_real_distribution<double> m_angle;
    Position m_center;

};

#endif /* RANDOMANULUSLOCATOR_H */

