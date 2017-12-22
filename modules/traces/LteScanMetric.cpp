#include "LteScanMetric.h"
#include "Log.h"

#include <iomanip>
#include <boost/filesystem.hpp>


LOG_REGISTER_MODULE("LteScanMetric");

LteScanMetric::LteScanMetric(std::string const& path, std::string const& tail)
: m_iter(0u)
, m_usersTrace(true, path, "users-scan", tail)
, m_enbsTrace(false, path, "cells-location", tail)
{
    BEG END;
}

void
LteScanMetric::SetIteration(std::uint32_t iter)
{
    BEG;
    m_iter = iter;
    END;
}

void
LteScanMetric::operator()(gnsm::Vec_t<User> users, gnsm::Vec_t<LteEnb> enbs)
{

    if (m_iter == 1u)
    {
        PrintCells(enbs);
    }
    PrintUsers(users);
}

void
LteScanMetric::PrintCells(gnsm::Vec_t<LteEnb> const& enbs)
{
    BEG;
    m_enbsTrace.Create();
    for (auto& enb_ : enbs)
    {
        for (auto i = 0u; i < enb_->ReadCells().size(); ++i)
        {
            auto x_ = enb_->ReadPosition().GetX();
            auto y_ = enb_->ReadPosition().GetY();
            m_enbsTrace.Write(std::to_string(x_) + "\t" + std::to_string(y_)+ "\n");
        }
    }
    m_enbsTrace.Close();
    END;
}

void
LteScanMetric::PrintUsers(gnsm::Vec_t<User> const& users)
{
    BEG;
    m_usersTrace.SetIter(m_iter);
    m_usersTrace.Create();
    
    for (auto& user_ : users)
    {
        m_usersTrace.Write(MakeUserStr(user_));
    }
    
    m_usersTrace.Close();
    END;
}

std::string
LteScanMetric::MakeUserStr(gnsm::Ptr_t<User> const& user)
{
    BEG;
    std::stringstream ss_;
    auto x_ = user->ReadPosition().GetX();
    auto y_ = user->ReadPosition().GetY();
    
    ss_ << std::setw(10) << x_ << '\t' << std::setw(10) << y_ << '\t';
    
    for (auto& item_ : user->GetLteDev()->ReadOrderedCellsDl())
    {
        ss_ << std::setw(10) << item_.m_rsrp.GetDbm() << '\t';
    }
    ss_ << '\n';    
    
    END;
    return ss_.str();
}