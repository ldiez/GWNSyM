#include <iomanip>

#include "LteUsersConnection.h"
#include "Log.h"
#include "Trace.h"

LOG_REGISTER_MODULE("LteUsersConnection");

LteUsersConnection::LteUsersConnection(std::string const& path, std::string const& tail)
: m_trace(true, path, "user-connection", tail)
, m_iter(0u)
{
    BEG;
    END;
}

void 
LteUsersConnection::SetIteration ( std::uint32_t iter)
{
    BEG;
    m_iter = iter;
    END;
}

void 
LteUsersConnection::operator()(gnsm::Vec_t<User> users)
{
    BEG;
    m_trace.SetIter(m_iter);
    m_trace.Create();
    
    INFO ("Creating trace for ", users.size(), " users");
    for (auto& user_ : users)
    {
        m_trace.Write(MakeStr(user_));
    }
    
    m_trace.Close();
    END;
}

std::string 
LteUsersConnection::MakeStr (gnsm::Ptr_t<User> const& user)
{
    BEG;
    std::stringstream ss_;
    auto dev_ = user->GetLteDev();
    auto ncells_ = dev_->ReadDlConnList().size();
    auto demand_ = user->GetConnectionManager()->DlDemand().GetKbps();
    auto achieved_ = dev_->ReadDlConnInfo().m_traffic.GetKbps();
    auto nrbs_ = dev_->ReadDlConnInfo().m_rbs;
    
    ss_ << std::setw(10) << std::setfill(' ') << ncells_
            << std::setw(10) << std::setfill(' ') << demand_
            << std::setw(10) << std::setfill(' ') << achieved_
            << std::setw(10) << std::setfill(' ') << nrbs_
            << '\n';
    END;
    return ss_.str();
}

