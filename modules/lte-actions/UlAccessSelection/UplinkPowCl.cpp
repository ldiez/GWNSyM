#include "UplinkPowCl.h"
#include "Log.h"
#include "UplinkUtils.h"

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
    auto cell = GetServCell(u);
    auto pl = u->GetLteDev()->GetCellUl(cell).m_pl;
    auto snrTh = u->GetLteDev()->GetConfiguration().UlSinrTh();
    auto alpha = u->GetLteDev()->GetConfiguration().GetAlpha();
    auto p = GetNoise(u, cell) + u->GetLteDev()->GetPrevUlInterf(); // noise plus interference
    p.Amp(snrTh).Amp(units::dB(alpha * pl.RawVal()));
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
