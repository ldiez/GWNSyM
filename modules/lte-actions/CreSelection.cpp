#include "CreSelection.h"
#include "Log.h"

LOG_REGISTER_MODULE("CreSelection");


CreSelection::CreSelection(units::dB picoCre, units::dB microCre, units::dB macroCre)
: m_creMap{
    {EnbType::MACRO, macroCre},
    {EnbType::MICRO, microCre},
    {EnbType::PICO, picoCre}
}
{
    BEG END;
}

std::pair<gnsm::Ptr_t<LteCell>, Power>
CreSelection::Select(LteUe::SensedValues_t const& sensed)
{
    Power maxRsrp_;
    Power actualRsrp_;
    gnsm::Ptr_t<LteCell> cell_;

    for (auto& item_ : sensed)
    {
        MSG_ASSERT(item_.m_type != EnbType::NONE, "Unknown eNB type passed for CRE selection");
        auto rsrp_ = item_.m_rsrp;
        rsrp_.Amp(m_creMap.at(item_.m_type));
        INFO("Evaluating cell ", item_.m_cell, " with raw RSRP ", item_.m_rsrp.GetDbm(),
             "dBm and actual value ", rsrp_.GetDbm(), "dBm");

        if (rsrp_ > maxRsrp_)
        {
            maxRsrp_ = rsrp_;
            actualRsrp_ = item_.m_rsrp;
            cell_ = item_.m_cell;
        }
    }
    INFO("Best cell: ", cell_, " with RSRP value ", actualRsrp_.GetDbm(),
         "dBm [", maxRsrp_.GetDbm(), "dBm]");
    return
    {
        cell_, actualRsrp_
    };
}

