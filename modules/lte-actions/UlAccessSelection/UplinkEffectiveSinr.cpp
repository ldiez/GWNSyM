#include "UplinkEffectiveSinr.h"
#include "Log.h"
#include "UplinkUtils.h"


LOG_REGISTER_MODULE("UplinkEffectiveSinr");

UplinkEffectiveSinr::UplinkEffectiveSinr()
{
    BEGEND;
}

void
UplinkEffectiveSinr::operator()(gnsm::Vec_t<User> us)
{
    BEG;
    INFO("--------------------------")
    for (auto& u : us)
    {
        if (!isConnected(u->GetLteDev()))
        {
            INFO("User ", u->GetId(), " not connected!!!")
            continue;
        }
        auto servCell = GetServCell(u);
        Sinr snr;
        snr.AddInterference(GetNoise(u, GetServCell(u)));
        auto txPower = GetConnPow(u);
        txPower.Att(GetPathloss(u, servCell));
//        txPower.Att(GetShadow(u, servCell));
        
        snr.AddSignal(txPower);
        for (auto& ui : us)
        {
            if (!AreInterfering(u, ui))
            {
                continue;
            }
            snr.AddInterference(CalculateInterference(u, ui));
        }
        

        INFO("+++ Serving User ", u->GetId(), " with SINR ", snr.SinrLog(), " dB");
        //        INFO("Tx. Power = ", GetConnPow(u).GetMilliWatt(), " mW (" , GetConnPow(u).GetDbm(), " dBm)"); 
        //        INFO("Rx. Power = ", txPower.GetMilliWatt(), " mW (" , txPower.GetDbm(), " dBm)"); 
        //        INFO("L = ", units::To<units::linear>::Do(GetPathloss(u, servCell)).RawVal(), 
        //             " ( ", units::To<units::dB>::Do(GetPathloss(u, servCell)).RawVal(), " dB)");
        //        INFO("User ", u->GetId(), " -- Interf = ", snr.InterferenceMw(), " mW (", snr.InterferenceDbm(), " dBm)");
        //        INFO("Noise = ", snr.NoiseMw(), " mW (", snr.NoiseDbm()," dBm)");
        //        INFO("User ", u->GetId(), " TxPower = ", txPower.GetDbm(), " dBm")
        INFO("User ", u->GetId(), " SINR = ", snr.SinrLog())
        if (snr.SinrLin() < 0)
        {
            MSG_ASSERT(false, "Negative or 0 SINR, something went wrong!!")
        }

        u->GetLteDev()->UlSetSinr(snr);
    }
    INFO("--------------------------")
    END;
}