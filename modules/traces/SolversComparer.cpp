#include "SolversComparer.h"
#include <iomanip>
#include "Log.h"

LOG_REGISTER_MODULE("SolversComparer");

SolversComparer::SolversComparer(std::string&& respath, std::string&& modPath, double backgrounLoad,
                                 double interLoad, units::dB cre)
: m_gp(std::forward<std::string>(modPath), backgrounLoad, interLoad, cre)
, m_sa(backgrounLoad, interLoad, cre)
, m_trace(false, std::forward<std::string>(respath), "solvers-comparer")
{
    BEG END;
}

SolversComparer::~SolversComparer()
{
    BEG;
    if (m_assignGp.empty())
    {
        return;
    }
    
    m_trace.Create(std::ios::app);
    for (auto i_ = 0u; i_ < m_assignSa.size(); ++i_)
    {
        std::stringstream ss_;
        ss_ << std::setprecision(4u) << std::setw(10u) << m_assignGp.at(i_) << "\t"
                << std::setprecision(4u) << std::setw(10u) << m_assignSa.at(i_) << "\t"
                << std::setprecision(4u) << std::setw(10u) << m_diff.at(i_) << "\n";
        m_trace.Write(ss_.str());
    }
    m_trace.Close();

    END;
}

void
SolversComparer::operator()(gnsm::Vec_t<User> users, gnsm::Vec_t<LteCell> interCells)
{
    BEG;
    auto resGp_ = m_gp.GetSolution(users, interCells);
    auto resSa_ = m_sa.GetSolution(users, interCells);

    if (resGp_.empty())
    {
        INFO("Python solver was not able to solve it");
        return;
    }
    
    

    MSG_ASSERT(resGp_.size() == resSa_.size(), "Results do not have the same size");
    INFO("\t   GP   \t   SA   \t   diff");
    INFO("\t========\t========\t==========");

    for (auto i_ = 0u; i_ < resGp_.size(); ++i_)
    {
        auto sa_ = resSa_.at(i_);
        auto gp_ = resGp_.at(i_);

        m_assignGp.push_back(gp_);
        m_assignSa.push_back(sa_);
        m_diff.push_back(sa_ - gp_);

        std::stringstream ss_;
        ss_ << "\t"
                << std::setw(8) << std::setprecision(4) << gp_
                << "\t"
                << std::setw(8) << std::setprecision(4) << sa_
                << "\t"
                << gp_ - sa_
                << "\t";
        INFO(ss_.str());
    }
    END;
}



