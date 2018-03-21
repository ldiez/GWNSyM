#ifndef UPLINKPOWCL_H
#define UPLINKPOWCL_H

#include "CoreUtils.h"

class User;

class UplinkPowCl {
public:
    UplinkPowCl();

    void SetIteration(gnsm::Id_t it);
    void operator()(gnsm::Vec_t<User> u);

private:
    
    gnsm::Id_t m_currIter;


};

#endif /* UPLINKPOWCL_H */

