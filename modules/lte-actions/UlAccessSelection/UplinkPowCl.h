#ifndef UPLINKPOWCL_H
#define UPLINKPOWCL_H

#include "CoreUtils.h"

class User;

class UplinkPowCl {
public:
    
    enum class ClMode {
        SINR,
        INTER
    };
    
    UplinkPowCl(ClMode m = ClMode::SINR);

    void SetIteration(gnsm::Id_t it);
    void operator()(gnsm::Vec_t<User> u);

private:
    const ClMode m_mode;
    gnsm::Id_t m_currIter;


};

#endif /* UPLINKPOWCL_H */

