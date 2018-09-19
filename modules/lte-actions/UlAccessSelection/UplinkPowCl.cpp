#include "UplinkPowCl.h"
#include "Log.h"
#include "UplinkUtils.h"
#include "UplinkPowSimple.h"

LOG_REGISTER_MODULE("UplinkPowCl");

UplinkPowCl::UplinkPowCl(ClMode m) : m_mode(m)
{
    BEG;
    END;
}

void
UplinkPowCl::SetIteration(gnsm::Id_t it)
{
    BEG;
    m_currIter = it;
    //    INFO("");
    //    INFO("===================================================");
    if (it == 1 or it % 10 == 0)
    {
        UINFO("Iteration ", it);
    }
    END;
}

bool
inClConnected(gnsm::Ptr_t<User> u)
{
    auto prevConn = u->GetLteDev()->GetPrevUlConn();
    return (prevConn.m_power.GetWatt() != 0.0);
}

void
ClPowerControlInter(gnsm::Ptr_t<User> u)
{
    BEG;

    auto cell = GetServCell(u);
    auto pl = GetPathloss(u, cell);
    auto snrTh = u->GetLteDev()->GetConfiguration().UlSinrTh();
    auto alpha = u->GetLteDev()->GetConfiguration().GetAlpha();
    auto p = GetNoise(u, cell) + u->GetLteDev()->GetPrevUlInterf(); // noise plus interference
    p.Amp(snrTh).Amp(units::dB(alpha * pl.RawVal()));
//    if (p.GetMilliWatt() < 1e-4)
//    {
//        p = Power(units::MilliWatt(1e-4));
//    }
    u->GetLteDev()->UlSetPower(p);
    END;
}

void
ClPowerControlSinr(gnsm::Ptr_t<User> u)
{
    BEG;
    auto snrTh = u->GetLteDev()->GetConfiguration().UlSinrTh();
    auto prevUlPow = u->GetLteDev()->GetPrevUlConn().m_power;
    INFO("User ", u->GetId(), " Transmitted with ", prevUlPow.GetDbm(), "dBm")
            auto prevSnr = u->GetLteDev()->GetPrevUlConn().m_sinr;
    prevUlPow.Amp(snrTh).Att(units::dB(prevSnr.SinrLog()));
    INFO("User ", u->GetId(), " Transmits with ", prevUlPow.GetDbm(), "dBm")
    INFO("User ", u->GetId(), " deficit ", snrTh.RawVal() - prevSnr.SinrLog(), "dB")
//    if (prevUlPow.GetMilliWatt() < 1e-4)
//    {
//        prevUlPow = Power(units::MilliWatt(1e-4));
//    }

    if (prevUlPow > u->GetLteDev()->GetConfiguration().GetPmax())
    {
        prevUlPow = u->GetLteDev()->GetConfiguration().GetPmax();
    }

    u->GetLteDev()->UlSetPower(prevUlPow);
    END;
}

void
UplinkPowCl::operator()(gnsm::Vec_t<User> us)
{
    BEG;
    for (auto& u : us)
    {
        if (!isConnected(u->GetLteDev()))
        {
            //            UINFO("User ", u->GetId(), " not connected!! ");
            continue;
        }
                INFO("");
                INFO("User ", u->GetId(), " transmitted with ", u->GetLteDev()->GetPrevUlConn().m_power.GetDbm(), " dBm");
                INFO("User ", u->GetId()," At ", u->GetPosition());
                INFO("User ", u->GetId(), " had interference ", u->GetLteDev()->GetPrevUlInterf().GetDbm(), " dBm");
                INFO("User ", u->GetId(), " got SINR ", u->GetLteDev()->GetPrevUlConn().m_sinr.SinrLog(), " dB");


        if (m_currIter == 1 or u->GetLteDev()->GetPrevUlConn().m_power.GetMilliWatt() == 0)
        {
            //            std::cout << "  User" << u->GetId() << " OL " << std::endl;
            OlPowerControl(u);
        }
        else if (m_mode == ClMode::INTER)
        {
            //            std::cout << "  User" << u->GetId() << " CL INTER " << std::endl;
            ClPowerControlInter(u);
        }
        else if (m_mode == ClMode::SINR)
        {
            //            std::cout << "  User" << u->GetId() << " CL SINR " << std::endl;
            ClPowerControlSinr(u);
        }
        //        INFO("User ", u->GetId(), " transmits with ", u->GetLteDev()->GetUlConnInfo().m_power.GetDbm(), " dBm");
    }
    END;
}
