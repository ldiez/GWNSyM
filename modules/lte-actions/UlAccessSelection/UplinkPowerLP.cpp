#include "UplinkPowerLP.h"
#include "Log.h"
#include "User.h"
#include "UplinkUtils.h"
#include "Chrono.h"

LOG_REGISTER_MODULE("UplinkPowerLP")
constexpr int output_handler(void *info, const char *s)
{
}

std::string GlpStatStr(int stat)
{
    return (stat == GLP_UNDEF ? "UNDEFINED" : stat == GLP_FEAS ? "FEASIBLE" :
            stat == GLP_INFEAS ? "INFEASIBLE" : stat == GLP_NOFEAS ?
            "NONFEASIBLE" : stat == GLP_OPT ? "OPTIMAL" : "UNBOUNDED");
}

UplinkPowerLP::UplinkPowerLP() : m_lp(nullptr)
{
    BEGEND;
}

UplinkPowerLP::~UplinkPowerLP()
{
    BEG;
    if (m_lp != nullptr)
    {
        glp_delete_prob(m_lp);
    }
    END;
}

void
UplinkPowerLP::SetIteration(gnsm::Id_t it)
{
    BEG;
    UINFO("Iteration ", it);
    END;
}

void
UplinkPowerLP::Clear()
{
    BEG;
    m_targetUsers.clear();
    m_vecAs.clear();
    m_coefs.clear();
    m_max.clear();
    m_sol.clear();
    m_matBs.clear();
    if (m_lp != nullptr)
    {

        glp_delete_prob(m_lp);
    }
    m_lp = glp_create_prob();
    MSG_ASSERT(m_lp != nullptr, "Error creating the problem!!")
    glp_set_prob_name(m_lp, "ul-allocator");
    glp_set_obj_name(m_lp, "linear-ls");
    glp_set_obj_dir(m_lp, GLP_MIN);
    glp_term_hook(output_handler, nullptr);
    END;
}

void
UplinkPowerLP::FilterUsers(gnsm::Vec_t<User> users)
{
    BEG;

    for (auto& u : users)
    {
        if (isConnected(u->GetLteDev()))
        {
            m_targetUsers.push_back(u);
        } else 
        {
            INFO ("User ", u->GetId(), " is NOT connected")
        }
    }
    END;
}

void
UplinkPowerLP::Map2problem(void)
{
    BEG;
    // Getting coefficients
    auto idx = 0u;
    for (auto& us : m_targetUsers)
    {
        std::vector<double> aux;
        auto idx2 = 0;
        for (auto& ui : m_targetUsers)
        {
            if (us == ui)
            {
                aux.push_back(-1);
            }
            else if (AreInterfering(us, ui))
            {
                aux.push_back(GetB(us, ui));
            }
            else
            {

                aux.push_back(0);
            }
            ++idx2;
        }
        m_vecAs.push_back(GetA(us));
        m_coefs.push_back(1);
        m_matBs.push_back(aux);

        auto nrbs = us->GetLteDev()->GetUlConnInfo().m_rbs;
        auto maxPow = us->GetLteDev()->GetConfiguration().GetPmax().GetMilliWatt();
        m_max.push_back(maxPow);
        ++idx;
    }
    END;
}

void
UplinkPowerLP::operator()(gnsm::Vec_t<User> us)
{
    BEG;
    Clear();
    FilterUsers(us);
    if (m_targetUsers.size() == 0)
    {

        return;
    }
    Map2problem();
    //    Print();
    AddRows();
    AddCols();
    SetCoefs();
    Solve();
    SetPower();
    END;
}

double
UplinkPowerLP::GetA(gnsm::Ptr_t<User> u) const
{
    BEG;
    auto sCell = GetServCell(u);
    auto snrTh = units::To<units::linear>::Do(u->GetLteDev()->GetConfiguration().UlSinrTh()).RawVal();
    auto noise = GetNoise(u, sCell).GetMilliWatt();
    auto pl = units::To<units::linear>::Do(GetPathloss(u, sCell)).RawVal();
    END;
    auto ret = -(snrTh * noise * pl);
    if (ret > 0)
    {
        INFO("");
        INFO("+++ A of user ", u->GetId());
        INFO("S_th = ", snrTh);
        INFO("Noise = ", noise);
        INFO("L_{i, b(i)} = ", pl);
        MSG_ASSERT(ret <= 0, "A <= 0");
    }
    return ret;
}

double
UplinkPowerLP::GetB(gnsm::Ptr_t<User> uSer, gnsm::Ptr_t<User> uInt) const
{
    BEG;
    auto sCell = GetServCell(uSer);
    auto pl_user_scell = units::To<units::linear>::Do(GetPathloss(uSer, sCell)).RawVal();
    auto pl_uint_scell = units::To<units::linear>::Do(GetPathloss(uInt, sCell)).RawVal();
    auto snrTh = units::To<units::linear>::Do(uSer->GetLteDev()->GetConfiguration().UlSinrTh()).RawVal();
    auto ratio = GetInterferenceRatio(sCell, uInt);

    END;
    auto ret = (snrTh * ratio * (pl_user_scell / pl_uint_scell));
    if (ret < 0)
    {
        INFO("");
        INFO("+++ B of servUser ", uSer->GetId(), " from ", uInt->GetId());
        INFO("S_th = ", snrTh);
        INFO("L_{i, b(i)} = ", pl_user_scell);
        INFO("L_{j, b(i)} = ", pl_uint_scell);
        INFO("Ratio = ", ratio);
        MSG_ASSERT(ret >= 0, "B >= 0");
    }
    return ret;
}

void
UplinkPowerLP::AddRows(void)
{
    BEG;
    MSG_ASSERT(m_lp != nullptr, "Empty problem!!");
    MSG_ASSERT(m_targetUsers.size() == m_vecAs.size(), "Bad problem storage!!");
    glp_add_rows(m_lp, m_vecAs.size());
    auto ctr = 1u;
    for (auto& item : m_vecAs)
    {
        glp_set_row_bnds(m_lp, ctr, GLP_UP, 0.0, item);
        ++ctr;
    }
    END;
}

void
UplinkPowerLP::AddCols(void)
{
    BEG;
    MSG_ASSERT(m_lp != nullptr, "Empty problem");
    glp_add_cols(m_lp, m_targetUsers.size());
    for (auto i = 0u; i < m_max.size(); ++i)
    {
        glp_set_col_bnds(m_lp, i + 1, GLP_LO, 0, m_max[i]);
    }
    END;
}

void
UplinkPowerLP::SetCoefs(void)
{
    BEG;
    auto nvars = m_targetUsers.size();
    for (auto it = 1u; it <= nvars; ++it)
    {// by rows
        glp_set_obj_coef(m_lp, it, m_coefs[it - 1]);
    }
    END;
}

void
UplinkPowerLP::Solve(void)
{
    BEG;
    std::vector<int> ia, ja;
    std::vector<double> ar;
    {// this is due to 0 indexing - Fortran inheritance
        ia.push_back(0);
        ja.push_back(0);
        ar.push_back(0);
    }
    auto nvars = m_targetUsers.size();

    for (auto i = 1u; i <= nvars; ++i)
    {// by rows
        for (auto j = 1u; j <= nvars; ++j)
        {
            ia.push_back(i);
            ja.push_back(j);
            ar.push_back(m_matBs[i - 1][j - 1]);
        }
    }

    glp_load_matrix(m_lp, ia.size() - 1, ia.data(), ja.data(), ar.data());
    {
        glp_smcp cp;
        glp_init_smcp(&cp);
        auto start = Now();
        glp_simplex(m_lp, &cp);
        INFO("Solving time ", Time2us(Now() - start), " us");
        auto status = glp_get_status(m_lp);
        if (status != GLP_OPT)
        {
            UWARN("Status: ", GlpStatStr(status));
            Clear();
            return;
        }
    }
    for (auto i = 1u; i <= nvars; ++i)
    {
        m_sol.push_back(glp_get_col_prim(m_lp, i));
    }
    END;
}

void
UplinkPowerLP::SetPower(void)
{
    BEG;
    for (auto i = 0u; i < m_sol.size(); ++i)
    {
        auto u = m_targetUsers[i];
        auto maxPowMw = u->GetLteDev()->GetConfiguration().GetPmax().GetMilliWatt();
        if (false)//m_sol[i] > maxPowMw)
        {
            u->GetLteDev()->UlSetPower(Power(units::MilliWatt(maxPowMw)));
        }
        else
        {
//            UINFO ("User ", u->GetId(), " to ", m_sol[i], "mw");
            u->GetLteDev()->UlSetPower(Power(units::MilliWatt(m_sol[i])));
        }
    }
    END;
}

void
UplinkPowerLP::Print(void)
{
    BEG;
    std::cout << "=================\n";
    std::cout << "min.\tsum ";
    auto idx = 0u;
    for (auto& c : m_coefs)
    {
        std::cout << c << " x_" << idx << " + ";
        ++idx;
    }
    std::cout << "\ns.t.\n";
    idx = 0u;
    for (auto& c : m_max)
    {
        std::cout << "\t0 <= x_" << idx << " <= " << c << "\n";
        ++idx;
    }
    idx = 0u;
    for (auto& cu : m_vecAs)
    {
        auto bs = m_matBs[idx];
        auto idx2 = 0;
        std::cout << "\t";
        for (auto b : bs)
        {
            std::cout << b << " x_" << idx2 << " + ";
            ++idx2;
        }
        std::cout << " <= " << cu;
        ++idx;
        std::cout << "\n";
    }
    END;
}