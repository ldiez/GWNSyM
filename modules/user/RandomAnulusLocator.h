#ifndef RANDOMANULUSLOCATOR_H
#define RANDOMANULUSLOCATOR_H

#include "User.h"
#include "Distance.h"
#include <random> 

class RandomAnulusLocator {
public:
    RandomAnulusLocator(Position center, units::m min, units::m max, std::uint32_t motionTime = 1);
    RandomAnulusLocator(Position center, units::m max, std::uint32_t motionTime = 1);
    RandomAnulusLocator(units::m max, std::uint32_t motionTime = 1);
    void operator () ( gnsm::Ptr_t<User> user);
    void SetIteration ( std::uint32_t iter );
private:
    std::default_random_engine m_re;
    std::uniform_real_distribution<double> m_range;
    std::uniform_real_distribution<double> m_angle;
    Position m_center;
    std::uint32_t m_motionTime;
    std::uint32_t m_iteration;

};

#endif /* RANDOMANULUSLOCATOR_H */

