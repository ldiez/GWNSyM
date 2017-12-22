#include "Log.h"
#include "SimulatedAnnealing.h"

namespace SA
{
LOG_REGISTER_MODULE("SimulatedAnnealing");

SimulatedAnnealing::SimulatedAnnealing(double initTemp)
: m_initTemp(initTemp), m_cool(m_initTemp)
{
    BEG;
    END;
}

SimulatedAnnealing&
SimulatedAnnealing::SetParam(std::string paramName, double value)
{
    BEG;
    SaParamEnum param_;
    try
    {
        param_ = SaParamMap.at(paramName);
    }
    catch (std::exception ex)
    {
        WARN("Parameter '", paramName, "' is not recognized and will be ignored");
        END;
        return *this;
    }

    switch (param_)
    {
    case SaParamEnum::INIT_TEMP:
        DBG("Setting initial temperature: ", value);
        m_initTemp = value;
        break;
    case SaParamEnum::FINAL_TEMP:
        DBG("Setting final temperature: ", value);
        MSG_ASSERT(value >= 0.0, "Final temperature is to be grater or equal to 0");
        m_finalTemp = value;
        break;
    case SaParamEnum::NUM_TEMPS:
        DBG("Setting number of temperatures: ", std::uint32_t(value));
        m_numTemp = value;
        break;
    case SaParamEnum::NUM_ITER:
        DBG("Setting number of iterations: ", std::uint32_t(value));
        m_numIter = value;
        break;
    case SaParamEnum::NUM_CYCLES:
        DBG("Setting number of cycles: ", std::uint32_t(value));
        m_cycles = value;
        break;
    case SaParamEnum::TIMES_TRIALS:
        DBG("Setting times of trials at each temperature: ", std::uint32_t(value));
        m_timesTrials = value;
        break;
    case SaParamEnum::GOAL:
        DBG("Setting goal: ", value);
        m_goal = value;
        break;
    case SaParamEnum::SEARCH_SPACE:
        DBG("Setting size of the search space: ", std::uint32_t(value));
        m_searchSpace = value;
        break;
    case SaParamEnum::COOLING_PARAM:
        DBG("Setting cooling parameter: ", value);
        m_coolingParam = value;
        break;
    case SaParamEnum::PERT_TYPE:
    case SaParamEnum::COOLING_TYPE:
        WARN("'", paramName, "' cannot be defined as a number");
        break;
    case SaParamEnum::PERT_SPREAD_FACTOR:
        DBG("Setting perturbation spread factor (double): ", value);
        m_pertSpreadFactor = value;
        break;
    };
    END;
    return *this;
}

SimulatedAnnealing&
SimulatedAnnealing::SetParam(std::string paramName, std::string value)
{
    BEG;
    SaParamEnum param_;
    try
    {
        param_ = SaParamMap.at(paramName);
    }
    catch (std::exception ex)
    {
        WARN("Parameter '", paramName, "' is not recognized and will be ignored");
        END;
        return *this;
    }

    switch (param_)
    {
    case SaParamEnum::INIT_TEMP:
        DBG("Setting initial temperature: ", value);
        m_initTemp = std::stod(value);
        break;
    case SaParamEnum::FINAL_TEMP:
        DBG("Setting final temperature: ", value);
        m_finalTemp = std::stod(value);
        MSG_ASSERT(m_finalTemp >= 0.0, "Final temperature is to be grater or equal to 0");
        break;
    case SaParamEnum::NUM_TEMPS:
        DBG("Setting number of temperatures: ", value);
        m_numTemp = std::stoi(value);
        break;
    case SaParamEnum::NUM_ITER:
        DBG("Setting number of iterations: ", value);
        m_numIter = std::stoi(value);
        break;
    case SaParamEnum::NUM_CYCLES:
        DBG("Setting number of cycles: ", value);
        m_cycles = std::stoi(value);
        break;
    case SaParamEnum::TIMES_TRIALS:
        DBG("Setting times of trials at each temperature: ", value);
        m_timesTrials = std::stoi(value);
        break;
    case SaParamEnum::GOAL:
        DBG("Setting goal: ", value);
        m_goal = std::stod(value);
        break;
    case SaParamEnum::SEARCH_SPACE:
        DBG("Setting size of the search space: ", value);
        m_searchSpace = std::stoi(value);
        break;
    case SaParamEnum::COOLING_PARAM:
        DBG("Setting cooling parameter: ", value);
        m_coolingParam = std::stod(value);
        break;
    case SaParamEnum::PERT_SPREAD_FACTOR:
        DBG("Setting perturbation spread factor (string): ", value);
        m_pertSpreadFactor = std::stod(value);
        break;
    case SaParamEnum::COOLING_TYPE:
        DBG("Setting cooling type: ", value);
        if (value.compare("EXP") != 0 && value.compare("LOG") != 0
                && value.compare("GEOM") != 0 && value.compare("LINEAR") != 0)
        {
            MSG_ASSERT(false, "Bad cooling type name, only accepted "
                       "'EXP', 'LOG', 'GEOM', 'LINEAR'");
        }
        m_coolingType = (value.compare("EXP") == 0 ? CoolingType_e::EXP :
                value.compare("LOG") == 0 ? CoolingType_e::LOG :
                value.compare("GEOM") == 0 ? CoolingType_e::GEOM :
                value.compare("LINEAR") == 0 ? CoolingType_e::LINEAR :
                CoolingType_e::LINEAR);
        break;

    case SaParamEnum::PERT_TYPE:
        DBG("Setting perturbation type ", value);
        if (value.compare("UNIFORM") != 0 && value.compare("NORMAL") != 0
                && value.compare("CAUCHY") != 0)
        {
            MSG_ASSERT(false, "Bad perturbation type name, only accepted "
                       "'UNIFORM', 'NORMAL', 'CAUCHY'");
        }
        m_pertType = (value.compare("UNIFORM") == 0 ? PertType_e::UNIFORM :
                value.compare("NORMAL") == 0 ? PertType_e::NORMAL :
                value.compare("CAUCHY") == 0 ? PertType_e::CAUCHY : PertType_e::CAUCHY);
        break;
    };
    END;
    return *this;
}

double
SimulatedAnnealing::Start(SaSolution& sol)
{
    BEG;
    m_cool.SetCoolingParameter(m_coolingParam);
    m_cool.SetCoolingType(m_coolingType);
    m_cool.SetFinalTemp(m_finalTemp);
    m_cool.SetNumTemps(m_numTemp);
    m_cool.SetSerachSpace(m_searchSpace);
    m_pert.Reset(m_pertSpreadFactor);
    m_pert.SetType(m_pertType);

    for (std::uint32_t it_ = 0; it_ < m_cycles; ++it_)
    {
        DBG("Anneal for cycle ", it_ + 1, " out of ", m_cycles);
        auto aux_ = Anneal(sol);
        if (aux_ <= m_goal)
        {
            DBG("Goal ", m_goal, " achieved [", aux_, "], close cycle ", it_);
            break;
        }
    }
    END;
    return sol.GetError();
}

double
SimulatedAnnealing::GetTemperature(std::uint32_t iter)
{
    BEG END;
    return m_cool.GetValue(iter);
}

bool
SimulatedAnnealing::IsAcceptable(double temp, double deltaError)
{
    BEG;
    END;
    m_accept.Reset(temp, m_K);
    return m_accept.Accept(deltaError);
}

double
SimulatedAnnealing::Anneal(SaSolution& sol)
{
    BEG;
    double error_ = sol.Initialize();
    if (error_ <= m_goal)
    {
        END;
        return error_;
    }
    else
    {
        m_K = EstimateK(sol, 1);
        for (std::uint32_t it_ = 0; it_ < m_numTemp; ++it_)
        {
            bool stop_ = ThermalEquilibrium(sol, it_);
            if (stop_) // goal reached
            {
                DBG("Getting out at temp iter ", it_);
                break;
            }
        }
    }
    END;

    return sol.GetError();
}

bool
SimulatedAnnealing::ThermalEquilibrium(SaSolution& sol, std::uint32_t iter)
{
    BEG;

    auto temp_ = GetTemperature(iter);
    m_pert.Reset(m_pertSpreadFactor * (1 - (m_initTemp - temp_) / m_initTemp));
    auto error_ = sol.GetError();
    auto stop_(false);
    auto trials_ = 0u;

    for (std::uint32_t it2_ = 0; it2_ < m_numIter;)
    {
        auto aux_ = sol.TryPerturb(temp_, m_initTemp, m_pert);
        double deltaError_ = aux_ - error_;

        if (IsAcceptable(temp_, deltaError_)) // deltaError_ < 0) //
        {
            sol.ApprovePerturb();
            error_ = sol.GetError();
            if ((std::abs(error_ - m_goal) < 1e-2) || error_ <= m_goal)
            {
                DBG("Goal reached, getting out!!");
                stop_ = true;
                break;
            }
            ++it2_;
        }
        else
        {
            ++trials_;
            INFO("Number of trials ", trials_, " max: ", m_timesTrials * m_numIter)
            if (trials_ >= m_timesTrials * m_numIter)
            {
                //                DBG("Nothing to do, getting out!!");
                //                if (it2_ >= (m_numIter / 50))
                //                {
                //                    stop_ = false;
                //                }
                //                else
                //                {
                //                    stop_ = true;
                //                }
                break;
            }
        }
    }
    END;

    return stop_;
}

double
SimulatedAnnealing::EstimateK(SaSolution& sol, std::uint32_t n)
{
    BEG;
    double error_(0.0);
    double sum_(0.0);
    double sumSq_(0.0);

    for (std::uint32_t it_ = 0; it_ < n; ++it_)
    {
        error_ = sol.TryPerturb(m_initTemp, m_initTemp, m_pert);
        sum_ += error_;
        sumSq_ += std::pow(error_, 2);
    }
    double variance_ = sumSq_ / (n - 1) - std::pow(sum_, 2) / (n * (n - 1));
    END;
    return -std::log(0.8) * m_initTemp / std::sqrt(variance_);
}
} // namespace SA