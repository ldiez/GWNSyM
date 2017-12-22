#include "ScdcSelection.h"
#include "Log.h"

LOG_REGISTER_MODULE("ScdcSelection");

ScdcSelection::ScdcSelection(std::string&& modPath, double backgroundLoad, double interLoad,
                             units::dB cre)
: m_solver(std::forward<std::string>(modPath), "scdc")
, m_lt(backgroundLoad, interLoad, cre)
{
    BEG;
    END;
}

void
ScdcSelection::operator()(gnsm::Vec_t<User> users, gnsm::Vec_t<LteCell> interCells)
{
    BEG;

    m_results = std::vector<double>(users.size());
    m_tempInfo = m_lt.Translate(users, interCells);
    Solve();
    Connect();
    END;
}

std::vector<double>
ScdcSelection::GetSolution(gnsm::Vec_t<User> users, gnsm::Vec_t<LteCell> interCells)
{
    BEG;
    m_results = std::vector<double>(users.size());
    m_tempInfo = m_lt.Translate(users, interCells);
    Solve();
    auto ret_ = m_results;
    END;
    return ret_;
}

void
ScdcSelection::Solve(void)
{
    BEG;
    PythonSolver::Vec servPow_;
    PythonSolver::Vec dem_;
    PythonSolver::Vec noise_;
    PythonSolver::Matrix intMat_;

    for (auto& item_ : m_tempInfo)
    {
        servPow_.push_back(item_.m_pow);
        dem_.push_back(item_.m_demand / (LTE::RbBw_s.RawVal() * LTE::BwEff_s));
        noise_.push_back(item_.m_interFloor + LTE::RbAwgnMwEff_s);
        intMat_.push_back(item_.m_interference);
    }
    m_results = m_solver.SolveScdc(servPow_, dem_, intMat_, noise_);
    END;
}

void
ScdcSelection::Connect(void)
{
    BEG;
    if (m_results.empty())
    {
        return;
    }

    auto ctr_ = 0u;
    for (auto& user_ : m_tempInfo)
    {
        auto check_ = user_.m_currCell->AddUser(m_results.at(ctr_));
        MSG_ASSERT(check_, "Cell capacity exceeded!!");
        ++ctr_;
    }
    m_tempInfo.clear();
    m_results.clear();
    END;
}

void
ScdcSelection::PrintTempInfo(void) const
{
    BEG;
    //    auto noise_ = NoisePowerWatt(Bandwidth(LTE::RbBw_s))*1000;
    //    std::stringstream p_;
    //    std::stringstream d_;
    //    std::stringstream i_;
    //    std::stringstream n_;
    //    p_ << "P = [";
    //    d_ << "D = [";
    //    i_ << "I = [...\n";
    //    n_ << "N = [";
    //
    //    for (auto& item_ : m_tempInfo)
    //    {
    //        p_ << item_.m_pow << ", ";
    //        d_ << item_.m_demand << ", ";
    //        n_ << item_.m_interFloor + noise_ << ", ";
    //        i_ << "[";
    //        for (auto& it_ : item_.m_interference)
    //        {
    //            i_ << it_ << ", ";
    //        }
    //        i_ << "]; ... \n";
    //    }
    //    p_ << "];";
    //    d_ << "];";
    //    i_ << "];";
    //    n_ << "];";
    //
    //    std::cout << p_.str() << std::endl;
    //    std::cout << d_.str() << std::endl;
    //    std::cout << i_.str() << std::endl;
    //    std::cout << n_.str() << std::endl;
    END;
}

void
ScdcSelection::Check(void) const
{
    BEG;


    auto ctr_ = 0u;
    for (auto& item_ : m_tempInfo)
    {
        auto acc_ = 0.0;

        auto ctr2_ = 0u;
        for (auto& it_ : item_.m_interference)
        {
            acc_ += it_ * m_results.at(ctr2_);
            ++ctr2_;
        }
        auto cap_ = m_results.at(ctr_) * LTE::RbBw_s.RawVal() * LTE::BwEff_s
                * std::log2(item_.m_pow / (LTE::RbAwgnMwEff_s + item_.m_interFloor + acc_));
        std::cout << "User " << ctr_ << " with capacity " << cap_ << std::endl;
        ++ctr_;
    }

    END;
}