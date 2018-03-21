#include "UplinkPowSimple.h"
#include "User.h"
#include "lte-ae/LteEnb.h"
#include "Log.h"

LOG_REGISTER_MODULE("UplinkPowSimple");

void
NoPowerControl(gnsm::Ptr_t<User> u) {
    BEG;
    auto p0 = u->GetLteDev()->GetConfiguration().GetPmax();
    auto ulConInfo = u->GetLteDev()->GetUlConnInfo();
    auto ulConList = u->GetLteDev()->GetUlConnList();
    INFO("User ", u->GetId(), " with total tranmission power ",
            p0.GetMilliWatt(), " mW to share among ", ulConInfo.m_rbs, " PRBs");
    INFO("List of cells it is conntected to");
    for (auto& c : ulConList) {
        INFO(c->GetEnb()->GetId(), "-", c->GetId());
    }
    u->GetLteDev()->UlSetPower(Power(units::MilliWatt(p0.GetMilliWatt() / ulConInfo.m_rbs)));
    END;
}

void
OlPowerControl(gnsm::Ptr_t<User> u) {
    BEG;
    auto p0 = Power(units::Watt(NoisePowerWatt(LTE::RbBw_s.RawVal()*1e3)));
    auto nf = u->GetLteDev()->GetConfiguration().GetNoiseFigure();
    auto snrTh = u->GetLteDev()->GetConfiguration().UlSinrTh();
    auto alpha = u->GetLteDev()->GetConfiguration().GetAlpha();
    INFO ("=================");
    INFO ("User ", u->GetId());
    INFO("Noise = ", p0.GetDbm(), " dBm NF = ", nf, " dB and SINR_th = ", snrTh, " dB");
    p0.Amp(nf).Amp(snrTh);
    INFO("P01 = ", p0.GetDbm(), " dBm");
    auto ulConList = u->GetLteDev()->GetUlConnList();
    auto cell = ulConList.at(0);
    auto pl = u->GetLteDev()->GetCellUl(cell).m_pl;
    INFO("PL = ", pl, " dB \\alpha = ", alpha);
    auto p = p0.Amp(units::dB(alpha * pl.RawVal()));
    INFO("Transmission power per PRB ", p.GetDbm());
    u->GetLteDev()->UlSetPower(p);
    END;
}

UplinkPowSimple::UplinkPowSimple(PcMode pcm)
: m_pcMode(pcm) {
    BEGEND;
}

void
UplinkPowSimple::operator()(gnsm::Ptr_t<User> u) {
    BEG;

    switch (m_pcMode) {
        case PcMode::NoPc:
            NoPowerControl(u);
            break;
        case PcMode::OlPc:
            OlPowerControl(u);
            break;
    }
    END;
}

