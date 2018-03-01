#include "UplinkEffectiveSinr.h"
#include "Log.h"
#include "User.h"
#include "lte-ae/LteCell.h"
#include "LteUtils.h"

LOG_REGISTER_MODULE("UplinkEffectiveSinr")

UplinkEffectiveSinr::UplinkEffectiveSinr() {
    BEGEND;
}

bool
isConnected(gnsm::Ptr_t<User> u) {
    auto servCells = u->GetLteDev()->GetUlConnList();
    return (servCells.size() >= 1);
}

gnsm::Ptr_t<LteCell>
GetServCell(gnsm::Ptr_t<User> u) {
    MSG_ASSERT(isConnected(u), "The user is not connected");
    BEGEND;
    return u->GetLteDev()->GetUlConnList().at(0);
}

double
GetConnNrbs(gnsm::Ptr_t<User> u) {
    MSG_ASSERT(isConnected(u), "The user is not connected");
    BEGEND;
    return u->GetLteDev()->GetUlConnInfo().m_rbs;
}

Power
GetConnPow(gnsm::Ptr_t<User> u) {
    MSG_ASSERT(isConnected(u), "The user is not connected");
    BEGEND;
    return u->GetLteDev()->GetUlConnInfo().m_power;
}

units::dB
GetPl(gnsm::Ptr_t<User> u, gnsm::Ptr_t<LteCell> c) {
    MSG_ASSERT(isConnected(u), "The user is not connected");
    BEGEND;
    return u->GetLteDev()->GetCellUl(c).m_pl;
}

Power
CalculateInterference(gnsm::Ptr_t<User> uServ, gnsm::Ptr_t<User> uInt) {
    BEG;
    MSG_ASSERT(isConnected(uServ), "The served user is not connected");
    MSG_ASSERT(isConnected(uInt), "The interfering user is not connected");
    auto cServ = GetServCell(uServ);
    auto sizeServ = cServ->GetUlResources();
    auto sizeInt = GetServCell(uInt)->GetUlResources();
    auto nrbsInt = GetConnNrbs(uInt);

    auto ratio = sizeServ <= sizeInt ? 1 : sizeInt / sizeServ;
    ratio *= (nrbsInt / sizeInt);
    auto plIntServ = uInt->GetLteDev()->GetCellUl(cServ).m_pl;
    auto powInt = GetConnPow(uInt);

    INFO("====");
    INFO("== Served USER ", uServ->GetId(), " interfering one ", uInt->GetId());
    INFO("Transmission power ", powInt.GetDbm(),
            " dBm with PL ", plIntServ,
            " and ratio ", ratio);
    powInt.Att(plIntServ).Amp(ratio);
    INFO ("Total interference ", powInt.GetMilliWatt(), " mW");
    END;
    return powInt;
}

void
UplinkEffectiveSinr::operator()(gnsm::Vec_t<User> us) {
    BEG;
    for (auto& u : us) {
        if (!isConnected(u)) {
            continue;
        }
        INFO("===============");
        INFO("===============");
        INFO("USER ", u->GetId());
        auto servCell = GetServCell(u);
        Sinr snr(Bandwidth(LTE::RbBw_s));
        auto txPower = u->GetLteDev()->GetUlConnInfo().m_power;
        INFO("Tx power ", txPower.GetMilliWatt(), " mW/PRB (", txPower.GetDbm(), " dBm)");
        INFO("Serving PL ", GetPl(u, servCell), " dB")
        txPower.Att(GetPl(u, servCell));
        INFO("Rx power ", txPower.GetMilliWatt(), " mW/PRB (", txPower.GetDbm(), " dBm)");
        snr.AddSignal(txPower);
        for (auto& ui : us) {
            if (!isConnected(ui) or servCell->HasSameEnb(GetServCell(ui))) {
                continue;
            }
            snr.AddInterference(CalculateInterference(u, ui));
        }
        INFO("Serving User ", u->GetId(), " with SIRN ", snr.SinrLog(),
                " dB:\n\t signal = ", snr.SignalMw(), " mW; interf = ",
                snr.InterferenceMw(), " mW; noise = ", snr.NoiseMw(), " mW");
        u->GetLteDev()->UlSetSinr(snr);
    }
    END;
}


