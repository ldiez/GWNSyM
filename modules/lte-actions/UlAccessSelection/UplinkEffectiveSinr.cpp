#include "UplinkEffectiveSinr.h"
#include "Log.h"
#include "UplinkUtils.h"


LOG_REGISTER_MODULE("UplinkEffectiveSinr")

UplinkEffectiveSinr::UplinkEffectiveSinr() {
    BEGEND;
}

void
UplinkEffectiveSinr::operator()(gnsm::Vec_t<User> us) {
    BEG;
    for (auto& u : us) {
        if (!isConnected(u->GetLteDev())) {
            continue;
        }
        auto servCell = GetServCell(u);
        Sinr snr(Bandwidth(LTE::RbBw_s));
        auto txPower = GetConnPow(u);
        txPower.Att(GetPl(u, servCell));
        snr.AddSignal(txPower);
        for (auto& ui : us) {
            if (AreInterfering(u, ui)) {
                continue;
            }
            snr.AddInterference(CalculateInterference(u, ui));
        }
        
        INFO("+++ Serving User ", u->GetId(), " with SIRN ", snr.SinrLog(), " dB");
//        INFO("Power = ", GetConnPow(u).GetMilliWatt(), " mW"); 
//        INFO("L = ", units::To<units::linear>::Do(GetPl(u, servCell)).RawVal());
//        INFO("Interf = ", snr.InterferenceMw(), " mW");
//        INFO("Noise = ", snr.NoiseMw(), " mW");
        u->GetLteDev()->UlSetSinr(snr);
    }
    END;
}