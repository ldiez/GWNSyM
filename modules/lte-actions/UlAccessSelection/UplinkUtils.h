#ifndef UPLINKUTILS_H
#define UPLINKUTILS_H

#include "User.h"
#include "lte-ae/LteCell.h"
#include "LteUtils.h"

inline gnsm::Ptr_t<LteCell>
GetServCell(gnsm::Ptr_t<User> u) {
    MSG_ASSERT(isConnected(u->GetLteDev()), "The user is not connected");
    return u->GetLteDev()->GetUlConnList().at(0);
}

inline double
GetConnNrbs(gnsm::Ptr_t<User> u) {
    MSG_ASSERT(isConnected(u->GetLteDev()), "The user is not connected");
    return u->GetLteDev()->GetUlConnInfo().m_rbs;
}

inline Power
GetConnPow(gnsm::Ptr_t<User> u) {
    MSG_ASSERT(isConnected(u->GetLteDev()), "The user is not connected");
    return u->GetLteDev()->GetUlConnInfo().m_power;
}

inline units::dB
GetPl(gnsm::Ptr_t<User> u, gnsm::Ptr_t<LteCell> c) {
    MSG_ASSERT(isConnected(u->GetLteDev()), "The user is not connected");
    return u->GetLteDev()->GetCellUl(c).m_pl;
}

inline Power
GetNoise(gnsm::Ptr_t<User> u, gnsm::Ptr_t<LteCell> c) {
    MSG_ASSERT(isConnected(u->GetLteDev()), "The user is not connected");
    auto nf = c->GetConfiguration().GetNoiseFigure();
    auto n = Power(units::Watt(NoisePowerWatt(LTE::RbBw_s.RawVal()*1e3)));
    return n.Amp(nf);
}

inline units::dB
GetPathloss(gnsm::Ptr_t<User> u, gnsm::Ptr_t<LteCell> c) {
    return u->GetLteDev()->GetCellUl(c).m_pl;
}

inline double
GetInterferenceRatio(gnsm::Ptr_t<LteCell> cServ, gnsm::Ptr_t<User> uInt) {
    MSG_ASSERT(isConnected(uInt->GetLteDev()), "The interfering user is not connected");
    auto sizeServ = cServ->GetUlResources();
    auto sizeInt = GetServCell(uInt)->GetUlResources();
    auto nrbsInt = GetConnNrbs(uInt);
    auto ratio = sizeServ <= sizeInt ? 1 : sizeInt / sizeServ;
    ratio *= (nrbsInt / sizeInt);
    return ratio;
}

inline Power
CalculateInterference(gnsm::Ptr_t<User> uServ, gnsm::Ptr_t<User> uInt) {
    MSG_ASSERT(isConnected(uServ->GetLteDev()), "The served user is not connected");
    MSG_ASSERT(isConnected(uInt->GetLteDev()), "The interfering user is not connected");
    auto cServ = GetServCell(uServ);
    auto ratio = GetInterferenceRatio(cServ, uInt);
    auto plIntServ = GetPathloss(uInt, cServ);
    auto powInt = GetConnPow(uInt);
    powInt.Att(plIntServ).Amp(ratio);
    return powInt;
}

inline bool
AreInterfering(gnsm::Ptr_t<User> u1, gnsm::Ptr_t<User> u2) {
    if (u1 == u2) {
        return false;
    }
    if (!isConnected(u1->GetLteDev()) or !isConnected(u2->GetLteDev())) {
        return false;
    }
    return true;
    //    auto c1 = GetServCell(u1);
    //    auto c2 = GetServCell(u2);
    //    return (!c1->HasSameEnb(c2));
}

#endif /* UPLINKUTILS_H */

