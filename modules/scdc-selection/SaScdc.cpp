#include <deque>

#include "SaScdc.h"
#include "SimulatedAnnealing.h"
#include "gnuplot_i.h"

#include "Log.h"


LOG_REGISTER_MODULE("SaScdc");

SaScdc::SaScdc(UtilityCaller_t uc)
: m_utilityCaller(uc)
, m_ic(nullptr)
, m_bestError(std::numeric_limits<double>::max())
{
    BEG END;
}

SaScdc::~SaScdc()
{
    BEG END;
}

void
SaScdc::SetCaller(UtilityCaller_t uc)
{
    BEG;
    m_utilityCaller = uc;
    END;
}

void
SaScdc::SetInfo(InfoCarrier& ic)
{
    BEG;
    m_ic = &ic;
    END;
}

void
SaScdc::Anneal(void)
{
    BEG;
    MSG_ASSERT (m_utilityCaller != nullptr, "Utility caller is not set!!");
    MSG_ASSERT (m_ic != nullptr, "Info is not set!!");
    m_bestAssigment = std::vector<double>(m_ic->GetNumUnits(), 0.0);
    m_currAssigment = std::vector<double>(m_ic->GetNumUnits(), 0.0);
    m_evolution.clear();
    m_auxErrVec.clear();
    m_bestError = 0.0;
    
    LaunchAnnealing();
    LoadBest();

    INFO ("Best error ", m_bestError);
//    PlotResults();
    END;
}

void
SaScdc::LaunchAnnealing(void)
{
    BEG;
    SA::SimulatedAnnealing sa_(500.0);
    sa_.SetParam("FINAL_TEMP", 0.0)
            .SetParam("NUM_TEMPS", 300)
            .SetParam("NUM_ITER", 20)
            .SetParam("TIMES_TRIALS", 20)
            .SetParam("NUM_CYCLES", 1)
            .SetParam("GOAL", 1.0e-12)
            .SetParam("SEARCH_SPACE", 10)
            .SetParam("COOLING_PARAM", .6)
            .SetParam("COOLING_TYPE", "EXP")
            .SetParam("PERT_SPREAD_FACTOR", 1)
            .SetParam("PERT_TYPE", "CAUCHY");
    sa_.Start(*this);
    END;
}

void
SaScdc::LoadBest(void)
{
    BEG;
    auto n_ = m_ic->GetNumUnits();
    MSG_ASSERT(m_bestAssigment.size() == n_, "Sizes do not match");
    for (auto i_ = 0u; i_ < n_; ++i_)
    {
        m_ic->SetAssignment(i_, m_bestAssigment.at(i_));
    }
    END;
}

void 
SaScdc::UpdateBest (void)
{
    BEG;
    if (m_auxError < m_bestError)
    {
//        INFO ("Best from ", m_bestError, " to ", m_auxError);
        m_bestError = m_auxError;
        m_bestAssigment = m_currAssigment;
    }
    END;
}

/****************************** SA ***********************************************/
void
SaScdc::DoInitialize(void)
{
    BEG;
    auto n_ = m_ic->GetNumUnits();
    for (auto i_ = 0u; i_ < n_; ++i_)
    {
        auto assign_ = SimpleAssignment(i_);
        m_ic->SetAssignment(i_, assign_);
        m_bestAssigment.at(i_) = assign_;
    }
    m_bestError = m_utilityCaller(*m_ic);
    
    END;
}

void
SaScdc::DoTryPerturb(double temp, double initTemp, SA::Perturbation & pert)
{
    BEG;
    auto n_ = m_ic->GetNumUnits();

    for (auto i_ = 0u; i_ < n_; ++i_)
    {
        auto mutual_ = m_ic->GetMutualInter(i_);
        auto simple_ = SimpleAssignment(i_);
        if (mutual_ == 0.0)
        {
            m_ic->SetAssignment(i_, simple_);
        }
        else// if (rand() / RAND_MAX > 0.9)// the user has mutual interference
        {
            auto curr_ = m_ic->GetAssignment(i_);
            auto excess_ = m_ic->GetThputExcess(i_);
            auto seff_ = m_ic->GetSeff(i_);
            double ratio_ = temp / initTemp;
            auto pert_ = std::abs(pert.GetValue() * ratio_);
            auto new_ = curr_ - (excess_ / (seff_ * InfoCarrier::ResFactor)) * pert_;
            new_ = new_ < 0.0 ? 0.0 : new_;
            m_ic->SetAssignment(i_, new_);
        }
    }

    END;
}

void
SaScdc::DoApprovePerturb(void)
{
    BEG;
    auto n_ = m_ic->GetNumUnits();
    MSG_ASSERT(m_currAssigment.size() == n_, "Sizes do not match");
    for (auto i_ = 0u; i_ < n_; ++i_)
    {
        m_currAssigment.at(i_) = m_ic->GetAssignment(i_);
    }
    m_evolution.push_back(m_auxError);
    UpdateBest();
    END;
}

double
SaScdc::ComputeError(void)
{
    BEG END;
    return m_utilityCaller(*m_ic);
}

double
SaScdc::ComputeAuxError(void)
{
    BEG END;
    
    auto err_ = m_utilityCaller(*m_ic);
    m_auxErrVec.push_back(err_);
    return err_;
}

void
SaScdc::DoPlotResults(void)
{
    BEG;
    auto size_ = m_evolution.size();
    std::cout << std::endl;
    std::cout << "===================================================================" << std::endl;
    std::cout << "Size of evolution " << size_ << std::endl;
    std::cout << "Best error: " << m_bestError << std::endl;
    std::cout << "===================================================================" << std::endl;
    std::cout << std::endl;

    if (m_evolution.size() > 5u)
    {
        Gnuplot gp2_;
        gp2_.set_style("points")
                .set_title("Accepted evolution")
                .set_xrange(0, size_)
                .set_grid()
                .plot_x(m_evolution, "X variable");
        wait_for_key();
    }
    
//    if (m_auxErrVec.size() > 5u)
//    {
//        Gnuplot gp2_;
//        gp2_.set_style("points")
//                .set_title("Trials evolution")
//                .set_xrange(0, m_auxErrVec.size())
//                .set_grid()
//                .plot_x(m_auxErrVec, "X variable");
//        wait_for_key();
//    }
    END;
}

double
SaScdc::SimpleAssignment(std::uint32_t i) const
{
    BEG;
    auto excess_ = m_ic->GetThputExcess(i);
    auto seff_ = m_ic->GetSeff(i);
    auto curr_ = m_ic->GetAssignment(i);
    auto assign_ = curr_ - (excess_ / (seff_ * InfoCarrier::ResFactor));
    END;
    return assign_;
}