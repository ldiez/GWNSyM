#include "UplinkPowerLP.h"
#include "Log.h"
#include "User.h"
#include "UplinkUtils.h"
#include "Chrono.h"

LOG_REGISTER_MODULE("UplinkPowerLP")
constexpr int output_handler(void *info, const char *s) {
}

std::string GlpStatStr(int stat) {
    return (stat == GLP_UNDEF ? "UNDEFINED" : stat == GLP_FEAS ? "FEASIBLE" :
            stat == GLP_INFEAS ? "INFEASIBLE" : stat == GLP_NOFEAS ?
            "NONFEASIBLE" : stat == GLP_OPT ? "OPTIMAL" : "UNBOUNDED");
}

UplinkPowerLP::UplinkPowerLP() {
    BEG;
    m_lp = glp_create_prob();
    glp_set_obj_dir(m_lp, GLP_MIN);
    glp_term_hook(output_handler, nullptr);
    END;
}

UplinkPowerLP::~UplinkPowerLP() {
    BEG;
    if (m_lp != nullptr) {
        glp_delete_prob(m_lp);
    }
    END;
}

void
UplinkPowerLP::Clear() {
    BEG;
    m_targetUsers.clear();
    m_vecAs.clear();
    m_matBs.clear();
    m_coefs.clear();
    m_sol.clear();
    END;
}

void
UplinkPowerLP::FilterUsers(gnsm::Vec_t<User> users) {
    BEG;

    for (auto& u : users) {
        if (isConnected(u->GetLteDev())) {
            m_targetUsers.push_back(u);
        }
    }
    END;
}

void
UplinkPowerLP::Map2problem(void) {
    BEG;
    // Getting coefficients
    auto idx = 0u;
    for (auto& us : m_targetUsers) {
        std::vector<double> aux;
        auto idx2 = 0;
        for (auto& ui : m_targetUsers) {
            if (us == ui) {
                aux.push_back(-1);
            } else if (AreInterfering(us, ui)) {
                aux.push_back(GetB(us, ui));
            } else {
                aux.push_back(0);
            }
            ++idx2;
        }
        m_vecAs.push_back(-GetA(us));
        m_coefs.push_back(1);
        m_matBs.push_back(aux);

        auto nrbs = us->GetLteDev()->GetUlConnInfo().m_rbs;
        auto maxPow = us->GetLteDev()->GetConfiguration().GetPmax().GetMilliWatt();
        m_max.push_back(maxPow / nrbs);
        ++idx;
    }
    END;
}

void
UplinkPowerLP::operator()(gnsm::Vec_t<User> us) {
    BEG;
    Clear();
    FilterUsers(us);
    Map2problem();
    //    Print();
    AddRows();
    AddCols();
    SetCoefs();
    Solve();
    SetPower();
    Clear();
    END;
}

double
UplinkPowerLP::GetA(gnsm::Ptr_t<User> u) const {
    BEG;
    auto sCell = GetServCell(u);
    auto snrTh = units::To<units::linear>::Do(u->GetLteDev()->GetConfiguration().UlSinrTh()).RawVal();
    auto noise = GetNoise(u, sCell).GetMilliWatt();
    auto pl = units::To<units::linear>::Do(GetPathloss(u, sCell)).RawVal();

    //        INFO("+++ A of user ", u->GetId());
    //        INFO("S_th = ", snrTh);
    //        INFO("Noise = ", noise);
    //        INFO("L_{i, b(i)} = ", pl);
    END;
    return (snrTh * noise * pl);
}

double
UplinkPowerLP::GetB(gnsm::Ptr_t<User> uSer, gnsm::Ptr_t<User> uInt) const {
    BEG;
    auto sCell = GetServCell(uSer);
    auto pl_user_scell = units::To<units::linear>::Do(GetPathloss(uSer, sCell)).RawVal();
    auto pl_uint_scell = units::To<units::linear>::Do(GetPathloss(uInt, sCell)).RawVal();
    auto snrTh = units::To<units::linear>::Do(uSer->GetLteDev()->GetConfiguration().UlSinrTh()).RawVal();

    auto ratio = GetInterferenceRatio(sCell, uInt);

    //    INFO ("+++ B of servUser ", uSer->GetId(), " from ", uInt->GetId());
    //    INFO("S_th = ", snrTh);
    //    INFO("L_{i, b(i)} = ", pl_user_scell);
    //    INFO("L_{j, b(i)} = ", pl_uint_scell);
    //    INFO("Ratio = ", ratio);
    END;
    return (snrTh * ratio * (pl_user_scell / pl_uint_scell));
}

void
UplinkPowerLP::AddRows(void) {
    BEG;
    MSG_ASSERT(m_lp != nullptr, "Empty problem!!");
    MSG_ASSERT(m_targetUsers.size() == m_vecAs.size(), "Bad problem storage!!");
    glp_add_rows(m_lp, m_vecAs.size());
    auto ctr = 1u;
    for (auto& item : m_vecAs) {
        glp_set_row_bnds(m_lp, ctr, GLP_UP, 0.0, item);
        ++ctr;
    }
    END;
}

void
UplinkPowerLP::AddCols(void) {
    BEG;
    MSG_ASSERT(m_lp != nullptr, "Empty problem");
    glp_add_cols(m_lp, m_targetUsers.size());
    for (auto i = 1u; i <= m_max.size(); ++i) {
        glp_set_col_bnds(m_lp, i, GLP_LO, 0.0, m_max[i]);
    }
    END;
}

void
UplinkPowerLP::SetCoefs(void) {
    BEG;
    auto nvars = m_targetUsers.size();
    for (auto it = 1u; it <= nvars; ++it) {// by rows
        glp_set_obj_coef(m_lp, it, m_coefs[it - 1]);
    }
    END;
}

void
UplinkPowerLP::Solve(void) {
    BEG;
    int ia[1 + 1000], ja[1 + 1000];
    double ar[1 + 1000];
    auto nvars = m_targetUsers.size();
    auto ctr = 1u;
    for (auto i = 1u; i <= nvars; ++i) {// by rows
        for (auto j = 1u; j <= nvars; ++j) {
            ia[ctr] = i;
            ja[ctr] = j;
            ar[ctr] = m_matBs[i - 1][j - 1];
            ++ctr;
        }
    }
    glp_load_matrix(m_lp, ctr - 1, ia, ja, ar);
    {
        glp_simplex(m_lp, nullptr);
        auto status = glp_get_status(m_lp);
        if (status != GLP_OPT) {
            WARN("Status: ", GlpStatStr(status))
        }
    }
    for (auto i = 1u; i <= nvars; ++i) {
        INFO("User ", i - 1, " with ", glp_get_col_prim(m_lp, i), " mW");
        m_sol.push_back(glp_get_col_prim(m_lp, i));
    }
    END;
}

void
UplinkPowerLP::SetPower(void) {
    BEG;
    for (auto i = 0u; i < m_sol.size(); ++i) {
        auto u = m_targetUsers[i];
        u->GetLteDev()->UlSetPower(Power(units::MilliWatt(m_sol[i])));
    }
    END;
}

void
UplinkPowerLP::Print(void) {
    BEG;
    std::cout << "=================\n";
    std::cout << "min.\tsum ";
    auto idx = 0u;
    for (auto& c : m_coefs) {
        std::cout << c << " x_" << idx << " + ";
        ++idx;
    }
    std::cout << "\ns.t.\n";
    idx = 0u;
    for (auto& c : m_max) {
        std::cout << "\t0 <= x_" << idx << " <= " << c << "\n";
        ++idx;
    }
    idx = 0u;
    for (auto& cu : m_vecAs) {
        auto bs = m_matBs[idx];
        auto idx2 = 0;
        std::cout << "\t";
        for (auto b : bs) {
            std::cout << b << " x_" << idx2 << " + ";
            ++idx2;
        }
        std::cout << " <= " << cu;
        ++idx;
        std::cout << "\n";
    }
    END;
}