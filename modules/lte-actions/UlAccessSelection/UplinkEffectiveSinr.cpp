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
        auto txPower = u->GetLteDev()->GetUlConnInfo().m_power;
        txPower.Att(GetPl(u, servCell));
        snr.AddSignal(txPower);
        for (auto& ui : us) {
            if (AreInterfering(u, ui)) {
                continue;
            }
            snr.AddInterference(CalculateInterference(u, ui));
        }
        INFO("Serving User ", u->GetId(), " with SIRN ", snr.SinrLog(), " dB");
        u->GetLteDev()->UlSetSinr(snr);
    }
    END;
}