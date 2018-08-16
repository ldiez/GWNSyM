#include "UplinkConn.h"
#include "lte-user/LteUe.h"
#include "Log.h"
#include "UplinkUtils.h"

LOG_REGISTER_MODULE("UplinkConn");

UplinkConn::UplinkConn(Mode m, units::dB picoCre,
                       units::dB microCre, units::dB macroCre)
: m_mode(m)
, m_picoBias(picoCre)
, m_microbias(microCre)
, m_macroBias(macroCre)
{
    BEGEND;
}

void Assign(gnsm::Ptr_t<LteCell> c, gnsm::Ptr_t<User> u, double nrbs)
{
    if (c->AddUlUser(u->GetId(), nrbs))
    {
        u->GetLteDev()->UlConnect({c}, nrbs);
        u->GetLteDev()->DlConnect({c},
        {
                                  Traffic(u->GetConnectionManager()->DlDemand()), nrbs
        });
    }
    else
    {
        WARN("Cell ", c->GetId(), " is full for user ", u->GetId());
    }
}

void
UplinkConn::operator()(gnsm::Vec_t<User> users)
{
    BEG;
    /**
     * Each user has a target SINR.
     * SINR = P0 * PL(1-\alpha)/(NI)
     */
    if (m_mode == Mode::RSRP)
    {
        for (auto& user_ : users)
        {
            SelectRsrp(user_);
        }
    }
    else if (m_mode == Mode::PL)
    {
        for (auto& user_ : users)
        {
            SelectPl(user_);
        }
    }
    else
    {
        for (auto& user_ : users)
        {
            SelectCre(user_);
        }
    }
    END;
}

void
UplinkConn::SelectRsrp(gnsm::Ptr_t<User>& user)
{
    BEG;
    auto nrbs_ = GetDemandRbs(user);
    if (nrbs_ == 0)
    { // no traffic
        return;
    }
    auto cellInfo_ = user->GetLteDev()->GetOrderedCellDl(0);
    auto cell_ = cellInfo_.m_cell;

    if (!IfAffordableUlConnection(GetPathloss(user, cellInfo_.m_cell)))
    {
        return;
    }
    Assign(cell_, user, nrbs_);
    END;
}

void
UplinkConn::SelectPl(gnsm::Ptr_t<User>& user)
{
    BEG;
    auto nrbs_ = GetDemandRbs(user);
    if (nrbs_ == 0)
    { // no traffic
//        UINFO("User ", user->GetId(), " no traffic");
        return;
    }
    auto cellsInfo_ = user->GetLteDev()->GetOrderedCellsUl();
    if (cellsInfo_.size() == 0)
    {
//        UINFO("User ", user->GetId(), " no cells");
        return;
    }
    auto cellInfo_ = cellsInfo_.at(0);

    if (!IfAffordableUlConnection(cellInfo_.m_pl))
    {
//        UINFO("User ", user->GetId(), " not affordable");
        return;
    }
    auto cell_ = cellInfo_.m_cell;
//    UINFO("User ", user->GetId(), " to ", cellInfo_.m_enbId, "-", cellInfo_.m_cell->GetId(), " with PL ", cellInfo_.m_pl.RawVal(), " dB")
    Assign(cell_, user, nrbs_);
    END;
}

void
UplinkConn::SelectCre(gnsm::Ptr_t<User>& user)
{
    BEG;
    auto nrbs_ = GetDemandRbs(user);
    if (nrbs_ == 0)
    { // no traffic
        return;
    }
    auto info = user->GetLteDev()->GetOrderedCellsDl();
    std::map<Power, gnsm::Ptr_t < LteCell>, std::greater < Power>> auxOrder;

    for (auto& item : info)
    {
        if (!IfAffordableUlConnection(GetPathloss(user, item.m_cell)))
        {
            continue;
        }
        auto rsrp = item.m_rsrp;
        switch (item.m_type)
        {
        case EnbType::PICO:
            rsrp.Amp(m_picoBias);
            break;
        case EnbType::MICRO:
            rsrp.Amp(m_microbias);
            break;
        case EnbType::MACRO:
        default:
            rsrp.Amp(m_macroBias);
            break;
        }
        auxOrder.insert({rsrp, item.m_cell});
    }



    auto cell_ = auxOrder.begin()->second;
    Assign(cell_, user, nrbs_);
    END;
}