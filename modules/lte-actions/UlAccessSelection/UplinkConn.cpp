#include "UplinkConn.h"
#include "lte-user/LteUe.h"
#include "Log.h"
#include "UplinkUtils.h"
#include "Chrono.h"

LOG_REGISTER_MODULE("UplinkConn");

UplinkConn::UplinkConn(Mode m)
: m_mode(m)
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
    auto start = Now();
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
    INFO(Time2ms(Now() - start), " ms");
    END;
}

void
UplinkConn::SelectRsrp(gnsm::Ptr_t<User>& user)
{
    BEG;

    if (!user->IsActive())
    { // no traffic
        return;
    }

    auto nrbs_ = GetDemandRbs(user);
    auto cellInfo_ = user->GetLteDev()->GetOrderedCellDl(0);
    auto cell_ = cellInfo_.m_cell;

    if (!IfAffordableUlConnection(GetPathloss(user, cellInfo_.m_cell)))
    {
        //        WARN("User ", user->GetId(), " not affordable");
        return;
    }
    //    auto dis = Get3dDistance(user->GetPosition(), cell_->GetEnb()->GetPosition());
    //    INFO("User ", user->GetId(), " to cell ", cell_->GetEnb()->GetId(), " - ", cell_->GetId(), " with distance ", dis.GetM(), " m")
    Assign(cell_, user, nrbs_);
    END;
}

void
UplinkConn::SelectPl(gnsm::Ptr_t<User>& user)
{
    BEG;
    if (!user->IsActive())
    { // no traffic
        return;
    }

    auto nrbs_ = GetDemandRbs(user);
    auto cellsInfo_ = user->GetLteDev()->GetOrderedCellsUl();
    if (cellsInfo_.size() == 0)
    {
        return;
    }
    auto cellInfo_ = cellsInfo_.at(0);

    if (!IfAffordableUlConnection(cellInfo_.m_pl))
    {
        return;
    }
    auto cell_ = cellInfo_.m_cell;
    //    auto dis = Get3dDistance(user->GetPosition(), cell_->GetEnb()->GetPosition());
    //    INFO("User ", user->GetId(), " to cell ", cell_->GetEnb()->GetId(), " - ", cell_->GetId(), " with distance ", dis.GetM(), " m")
    Assign(cell_, user, nrbs_);
    END;
}

void
UplinkConn::SelectCre(gnsm::Ptr_t<User>& user)
{
    BEG;
    if (!user->IsActive())
    { // no traffic
        WARN("User ", user->GetId(), " no traffic");
        return;
    }

    auto nrbs_ = GetDemandRbs(user);
    auto info = user->GetLteDev()->GetOrderedCellsDl();
    std::map<Power, gnsm::Ptr_t < LteCell>, std::greater < Power>> auxOrder;

    for (auto& item : info)
    {
        if (!IfAffordableUlConnection(GetPathloss(user, item.m_cell)))
        {
            //            UINFO("User ", user->GetId(), " not affordable");
            //            UINFO("Cell is ", item.m_cell->GetEnb()->GetId(), " - ", item.m_cell->GetId())
            continue;
        }
        auto rsrp = item.m_rsrp;
        units::dB bias(0);
        switch (item.m_type)
        {
        case EnbType::PICO:
            bias = user->GetLteDev()->GetConfiguration().GetBiasPico();
            break;
        case EnbType::MICRO:
            bias = user->GetLteDev()->GetConfiguration().GetBiasMicro();
            break;
        case EnbType::MACRO:
        default:
            bias = user->GetLteDev()->GetConfiguration().GetBiasMacro();
            break;
        }
        rsrp.Amp(bias);
        auxOrder.insert({rsrp, item.m_cell});
    }
    if (auxOrder.size() > 0)
    {
        auto cell_ = auxOrder.begin()->second;
        //    auto dis = Get3dDistance(user->GetPosition(), cell_->GetEnb()->GetPosition());
        //    INFO("User ", user->GetId(), " to cell ", cell_->GetEnb()->GetId(), " - ", cell_->GetId(), " with distance ", dis.GetM(), " m")
        Assign(cell_, user, nrbs_);
    }
    END;
}