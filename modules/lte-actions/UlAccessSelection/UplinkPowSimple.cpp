#include "UplinkPowSimple.h"
#include "User.h"
#include "lte-ae/LteEnb.h"
#include "Log.h"

LOG_REGISTER_MODULE("UplinkPowSimple")

UplinkPowSimple::UplinkPowSimple(PcMode pcm, double alpha)
: m_pcMode(pcm) {
    BEGEND;
}

void
UplinkPowSimple::operator()(gnsm::Ptr_t<User> u) {
    BEG;
    switch (m_pcMode) {
        case PcMode::NoPc:
            NoPowerControl(u);
            break;
        case PcMode::OlPc:
            ERROR ("Not implemented yet!");
            break;
    }
    END;

}

void
UplinkPowSimple::NoPowerControl(gnsm::Ptr_t<User> u) {
    BEG;
    auto p0 = u->GetLteDev()->GetConfiguration().GetPmax();
    auto ulConInfo = u->GetLteDev()->GetUlConnInfo();
    auto ulConList = u->GetLteDev()->GetUlConnList();

    INFO("User ", u->GetId(), " with total tranmission power ",
            p0.GetMilliWatt(), " mW to share among ", ulConInfo.m_rbs, " PRBs");

    INFO("List of cells it is conntected to");
    for (auto& c : ulConList) {
        INFO(c->GetEnb()->GetId(), "-", c->GetId());
    }

    u->GetLteDev()->UlSetPower(Power(units::MilliWatt(p0.GetMilliWatt() / ulConInfo.m_rbs)));
    END;
}