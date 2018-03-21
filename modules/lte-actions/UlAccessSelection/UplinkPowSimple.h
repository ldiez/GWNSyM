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

    UplinkPowSimple(PcMode pcm = PcMode::NoPc);
    void operator()(gnsm::Ptr_t<User> u);
private:
    PcMode m_pcMode;
};

void NoPowerControl(gnsm::Ptr_t<User> u);
void OlPowerControl(gnsm::Ptr_t<User> u);

#endif /* ULNOPC_H */