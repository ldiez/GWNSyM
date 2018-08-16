#include "UlClosestConn.h"
#include "lte-ae/LteEnb.h"
#include "User.h"
#include "Log.h"
#include "UplinkUtils.h"

LOG_REGISTER_MODULE("UlClosestConn");

UlClosestConn::UlClosestConn()
{
    BEGEND;
}

void
UlClosestConn::operator()(gnsm::Vec_t<User> users, gnsm::Vec_t<LteEnb> enbs)
{
    BEG;
    for (auto& u : users)
    {
        auto minDst = 1e6;
        gnsm::Ptr_t<LteCell> cell = nullptr;
        for (auto& e : enbs)
        {
            auto currDist = GetPlanarDistance(u->GetPosition(), e->GetPosition()).GetM();
            if (currDist < minDst)
            {
                cell = e->GetCell(1);
                MSG_ASSERT (cell != nullptr, "Nulled cell!!");
                minDst = currDist;
            }
        }
        
        auto nrbs_ = GetDemandRbs(u);
        u->GetLteDev()->UlConnect({cell}, nrbs_);
    }
    END;
}


