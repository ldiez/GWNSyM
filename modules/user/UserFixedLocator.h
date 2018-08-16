#ifndef USERFIXEDLOCATOR_H
#define USERFIXEDLOCATOR_H

#include "Position.h"
#include "Distance.h"

class User;
class UserFixedLocator {
public:
    UserFixedLocator(std::vector<Position>);
    void operator () (gnsm::Vec_t<User> us);
private:
    const std::vector<Position> m_pos;

};

#endif /* USERFIXEDLOCATOR_H */

