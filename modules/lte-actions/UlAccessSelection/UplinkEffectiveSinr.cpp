#include "UplinkEffectiveSinr.h"
#include "Log.h"
#include "UplinkUtils.h"


LOG_REGISTER_MODULE ("UplinkEffectiveSinr");

UplinkEffectiveSinr::UplinkEffectiveSinr() {
    BEGEND;
}

void
UplinkEffectiveSinr::operator()(gnsm::Vec_t<User> us) {
    BEG;
    for (auto& u : us) {
        INFO ("");
        INFO ("-->> Interference for user ", u->GetId());
        if (!isConnected(u->GetLteDev())) {
            INFO ("User ", u->GetId(), " not connected!!!")
            continue;
        }
        auto servCell = GetServCell(u);
        Sinr snr;
        snr.AddInterference(GetNoise(u, GetServCell(u)));
        auto txPower = GetConnPow(u);
        txPower.Att(GetPathloss(u, servCell));
        snr.AddSignal(txPower);
        for (auto& ui : us)
        {
            if (!AreInterfering(u, ui))
            {
                continue;
            }
            INFO("User ", ui->GetId(), " Interfeers user ", u->GetId());
            snr.AddInterference(CalculateInterference(u, ui));
        }
        
        INFO("+++ Serving User ", u->GetId(), " with SINR ", snr.SinrLog(), " dB");
        INFO("Tx. Power = ", GetConnPow(u).GetMilliWatt(), " mW (" , GetConnPow(u).GetDbm(), " dBm)"); 
        INFO("Rx. Power = ", txPower.GetMilliWatt(), " mW (" , txPower.GetDbm(), " dBm)"); 
        INFO("L = ", units::To<units::linear>::Do(GetPathloss(u, servCell)).RawVal(), 
             " ( ", units::To<units::dB>::Do(GetPathloss(u, servCell)).RawVal(), " dB)");
        INFO("Interf = ", snr.InterferenceMw(), " mW (", snr.InterferenceDbm()," dBm)" );
        INFO("Noise = ", snr.NoiseMw(), " mW (", snr.NoiseDbm()," dBm)");
        INFO ("User ", u->GetId(), " TxPower = ", txPower.GetDbm(), " dBm")
        INFO ("User ", u->GetId(), " SINR = ", snr.SinrLin())
//        if (snr.SinrLin() <= 0) {
//            
//            MSG_ASSERT (false, "Negative or 0 SINR, something went wrong!!")
//        }
        u->GetLteDev()->UlSetSinr(snr);
    }
    END;
}