#ifndef MOVEAWAYLOCATOR_H
#define MOVEAWAYLOCATOR_H


#include "Distance.h"
#include "Position.h"
#include <random> 

/**
 * In each iteration it moves in the X-axis direction
 */
class User;
class MoveAwayLocator {
public:
    
    MoveAwayLocator(Position init, units::m step);
    void SetIteration ( std::uint32_t iter );
    
    void operator() (gnsm::Vec_t<User> u);
private:
    const Position m_init;
    const units::m m_step;
    std::uint32_t m_iter;
    

};

#endif /* MOVEAWAYLOCATOR_H */

