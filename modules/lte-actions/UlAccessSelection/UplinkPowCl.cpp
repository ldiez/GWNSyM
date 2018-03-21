#include "UplinkPowCl.h"
#include "Log.h"
#include "User.h"
#include "lte-ae/LteEnb.h"


LOG_REGISTER_MODULE("UplinkPowCl");

UplinkPowCl::UplinkPowCl() {
    BEG;
    END;
}

void
UplinkPowCl::SetIteration(gnsm::Id_t it) {
    BEG;
    m_currIter = it;
    std::cout << "Iteration " << it << std::endl;
    INFO("Current iteration ", it);
    END;
}

bool
inClConnected(gnsm::Ptr_t<User> u) {
    auto prevConn = u->GetLteDev()->GetPrevUlConn();
    return (prevConn.m_power.GetWatt() != 0.0);
}

void
ClPowerControl(gnsm::Ptr_t<User> u) {
    BEG;
    auto p0 = Power(units::Watt(NoisePowerWatt(LTE::RbBw_s.RawVal()*1e3)));
    auto noise = p0;
    auto inter = u->GetLteDev()->GetPrevUlInterf();
    auto snrTh = u->GetLteDev()->GetConfiguration().UlSinrTh();
    auto alpha = u->GetLteDev()->GetConfiguration().GetAlpha();
    auto nf = u->GetLteDev()->GetConfiguration().GetNoiseFigure();
    p0.Amp(nf);
    p0 += inter; // noise plus interference
    INFO("==================");
    INFO("USER ", u->GetId());
    INFO("N+I = ", p0.GetDbm(), " dBm");
    INFO("N+I diff = ", p0.GetDbm() - noise.GetDbm(), " dB");
    p0.Amp(snrTh);
    auto ulConList = u->GetLteDev()->GetUlConnList();
    auto cell = ulConList.at(0);
    auto pl = u->GetLteDev()->GetCellUl(cell).m_pl;
    auto p = p0.Amp(units::dB(alpha * pl.RawVal()));
    INFO("Transmission power per PRB ", p.GetDbm(), "dBm");
    {// second approach
//        auto prevPow = u->GetLteDev()->GetPrevUlConn().m_power;
//        auto prevSinr = u->GetLteDev()->GetPrevUlConn().m_sinr;
//        INFO("+-+-Previous interference = ", prevSinr.InterferenceDbm(), " dBm");
//        auto deltaSinr = snrTh.RawVal() - prevSinr.SinrLog();
//        INFO("+-+-Previous SINR = ", prevSinr.SinrLog(), " dB");
//        INFO("+-+-Target SINR = ", snrTh.RawVal(), " dB");
//        INFO("+-+-\\Delta SINR = ", deltaSinr, " dB");
//        INFO("+-+-Previous power = ", prevPow.GetDbm(), " dBm");
//        INFO("+-+-New power = ", prevPow.Amp(units::dB(deltaSinr)).GetDbm(), " dBm");
    }
    u->GetLteDev()->UlSetPower(p);
    END;
}

void
UplinkPowCl::operator()(gnsm::Vec_t<User> us) {
    BEG;
    for (auto& u : us) {
        if (!isConnected(u->GetLteDev())) {
            continue;
        }
        ClPowerControl(u);
    }
    END;
}
