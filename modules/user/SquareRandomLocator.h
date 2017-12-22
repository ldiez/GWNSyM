#ifndef SQUARERANDOMLOCATOR_H
#define SQUARERANDOMLOCATOR_H

#include "User.h"
#include "Distance.h"
#include <random> 

class SquareRandomLocator
{
public:
    /**
     * \brief Set the scenario size. the users are deployed 
     * in a square from 0-side X 0-side
     * \param side --> Scenario side
     */
    SquareRandomLocator(units::m side);
    SquareRandomLocator(units::m min, units::m max);
    SquareRandomLocator(units::m xmin, units::m xmax, units::m ymin, units::m ymax);
    
    void operator () ( gnsm::Ptr_t<User> user);
    void SetIteration ( std::uint32_t iter );
private:
    std::default_random_engine m_re;
    std::uniform_real_distribution<double> m_xDist;
    std::uniform_real_distribution<double> m_yDist;
    
    
    

};

#endif /* SQUARERANDOMLOCATOR_H */

