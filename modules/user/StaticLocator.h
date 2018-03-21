#ifndef STATICLOCATOR_H
#define STATICLOCATOR_H

#include "CoreUtils.h"

class User;
class StaticLocator {
public:
    StaticLocator();
    void operator () ( gnsm::Ptr_t<User> user);
private:

};

#endif /* STATICLOCATOR_H */

