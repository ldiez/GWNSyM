#ifndef UPLINKEFFECTIVESINR_H
#define UPLINKEFFECTIVESINR_H

#include "CoreUtils.h"

class User;
class UplinkEffectiveSinr {
public:
    UplinkEffectiveSinr();
    
    void operator () (gnsm::Vec_t<User> us);
private:

};

#endif /* UPLINKEFFECTIVESINR_H */