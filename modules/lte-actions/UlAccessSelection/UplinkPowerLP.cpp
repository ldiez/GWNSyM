#include "UplinkPowerLP.h"
#include "Log.h"
#include "User.h"
#include "UplinkUtils.h"

LOG_REGISTER_MODULE("UplinkPowerLP")
constexpr int output_handler(void *info, const char *s) {
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
UplinkPowerLP::operator()(gnsm::Vec_t<User> us) {
    BEG;
    std::vector<double> as;
    std::vector<double> bs;
    for (auto& u : us) {
        auto a = GetA(u);
        for (auto& ui : us) {
            if (AreInterfering(u, ui)) {
                continue;
            }
            auto b = GetB(u, ui);
        }
    }
    END;
}

double
UplinkPowerLP::GetA(gnsm::Ptr_t<User> u) const {
    BEG;
    auto sCell = GetServCell(u);
    auto snrTh = u->GetLteDev()->GetConfiguration().UlSinrTh().RawVal();
    auto noise = GetNoise(u, sCell).GetMilliWatt();
    auto pl = GetPathloss(u, sCell).RawVal();
    END;
    return (snrTh * noise / pl);
}

double
UplinkPowerLP::GetB(gnsm::Ptr_t<User> uSer, gnsm::Ptr_t<User> uInt) const {
    BEG;
    auto sCell = GetServCell(uSer);
    auto iCell = GetServCell(uInt);
    auto pl_uint_scell = GetPathloss(uInt, sCell).RawVal();
    auto pl_uint_icell = GetPathloss(uInt, iCell).RawVal();
    auto snrTh = uSer->GetLteDev()->GetConfiguration().UlSinrTh().RawVal();
    auto ratio = GetInterferenceRatio(sCell, uInt);
    END;
    return (snrTh * ratio / (pl_uint_icell * pl_uint_scell));
}

void
UplinkPowerLP::AddRows(void) {
    BEG;
    END;
}

void
UplinkPowerLP::AddCols(void) {
    BEG;
    END;
}

void
UplinkPowerLP::SetCoefs(void) {
    BEG;
    END;
}