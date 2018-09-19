#include "UplinkPowSimple.h"
#include "Log.h"
#include "UplinkUtils.h"
#include "lte-ae/LteEnb.h"

LOG_REGISTER_MODULE("UplinkPowSimple");

void
NoPowerControl(gnsm::Ptr_t<User> u)
{
    BEG;
    auto p0 = u->GetLteDev()->GetConfiguration().GetPmax();
    auto ulConInfo = u->GetLteDev()->GetUlConnInfo();
    auto ulConList = u->GetLteDev()->GetUlConnList();
    INFO("User ", u->GetId(), " with total tranmission power ",
         p0.GetMilliWatt(), " mW to share among ", ulConInfo.m_rbs, " PRBs");
    INFO("List of cells it is conntected to");
    for (auto& c : ulConList)
    {
        INFO(c->GetEnb()->GetId(), "-", c->GetId());
    }
    u->GetLteDev()->UlSetPower(Power(units::MilliWatt(p0.GetMilliWatt() / ulConInfo.m_rbs)));
    END;
}

void
OlPowerControl(gnsm::Ptr_t<User> u)
{
    BEG;
    auto cell = GetServCell(u);
    auto pl = u->GetLteDev()->GetCellUl(cell).m_pl;
    auto snrTh = u->GetLteDev()->GetConfiguration().UlSinrTh();
    auto alpha = u->GetLteDev()->GetConfiguration().GetAlpha();
    auto p = GetNoise(u, cell).Amp(snrTh).Amp(units::dB(alpha * pl.RawVal()));
    
//    UINFO ("User",u->GetId(), " pathLoss ", pl.RawVal(), " dB", " tx power ", p.GetDbm(), " dBm");
    
    auto pmax = u->GetLteDev()->GetConfiguration().GetPmax();
    p = p > pmax ? pmax : p;
//    if (p.GetMilliWatt() < 1e-4) {
//        p = Power(units::MilliWatt(1e-4));
//    }
//    UINFO ("User ", u->GetId(), " transmits with ", p.GetDbm(), " dBm")
    u->GetLteDev()->UlSetPower(p);
    END;
}

UplinkPowSimple::UplinkPowSimple(PcMode pcm)
: m_pcMode(pcm)
{
    BEGEND;
}

void
UplinkPowSimple::SetIteration(std::uint32_t iter)
{
    BEG;
    if (iter % 100 == 0)
    {
        UINFO("Iteration ", iter);
    }
    END;
}

void
UplinkPowSimple::operator()(gnsm::Ptr_t<User> u)
{
    BEG;
    if (!isConnected(u->GetLteDev()))
    {
        return;
    }
    switch (m_pcMode)
    {
    case PcMode::NoPc:
        NoPowerControl(u);
        break;
    case PcMode::OlPc:
        OlPowerControl(u);
        break;
    }
    END;
}

