#ifndef UPLINKPOWSIMPLE_H
#define UPLINKPOWSIMPLE_H

#include "CoreUtils.h"

class User;

class UplinkPowSimple {
public:

    enum class PcMode {
        NoPc,
        OlPc
    };
    
    UplinkPowSimple(PcMode pcm = PcMode::NoPc, double alpha = 0);
    void operator()(gnsm::Ptr_t<User> u);
private:

    void NoPowerControl(gnsm::Ptr_t<User> u);
    PcMode m_pcMode;
};

#endif /* ULNOPC_H */