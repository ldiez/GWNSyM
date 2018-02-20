#include <algorithm>

#include "LteUe.h"
#include "Log.h"
#include "LteUtils.h"


LOG_REGISTER_MODULE("LteUe");

inline bool
SensedSort(LteUe::CellScan const& left, LteUe::CellScan const& right) {
    return left.m_rsrp > right.m_rsrp;
}

inline bool
EstimateSort(LteUe::UlLosses const& left, LteUe::UlLosses const& right) {
    return left.m_pl < right.m_pl;
}

LteUe::LteUe(gnsm::Id_t id)
: m_orderedDl(false)
, m_orderedUl(false)
, m_id(id) {
    BEG END;
}

void
LteUe::SetConfiguration(LteUeConf const& conf) {
    BEG END;
    m_conf = &conf;
}

LteUeConf const&
LteUe::ReadConfiguration(void) {
    BEG END;
    return *m_conf;
}

void
LteUe::AddCellInfo(gnsm::Id_t enbId, gnsm::Ptr_t<LteCell> cell, Power rsrp, units::dB ulPl, EnbType type) {
    BEG;
    INFO("Adding cell [", cell, "] of type ", type, "with Rx power ",
            rsrp.GetMilliWatt(), "mW");
    m_orderedDl = false;
    m_orderedUl = false;
    m_sensedDlPower.push_back({enbId, cell, rsrp, type});
    m_ulEstimates.push_back({enbId, cell, ulPl, type});
    END;
}

LteUe::SensedValues_t const&
LteUe::ReadOrderedCellsDl(void) {
    BEG;
    if (!m_orderedDl) {
        std::sort(m_sensedDlPower.begin(), m_sensedDlPower.end(), SensedSort);
        m_orderedDl = true;
    }
    END;

    return m_sensedDlPower;
}

LteUe::CellScan
LteUe::ReadCellDl(gnsm::Ptr_t<LteCell> const& c) {
    BEG;
    for (auto& item_ : m_sensedDlPower) {
        if (item_.m_cell == c) {
            return item_;
        }
    }
    END;
    MSG_ASSERT(false, "The cell required does not exist")
    return {};
}

LteUe::UlEstimate_t const&
LteUe::ReadOrderedCellsUl(void) {
    BEG;
    if (!m_orderedUl) {
        std::sort(m_ulEstimates.begin(), m_ulEstimates.end(), EstimateSort);
        m_orderedUl = true;
    }
    END;
    return m_ulEstimates;
}

LteUe::UlLosses
LteUe::ReadCellUl(const gnsm::Ptr_t<LteCell>& c) {
    BEG;

    for (auto& item_ : m_ulEstimates) {
        if (item_.m_cell == c) {
            return item_;
        }
    }
    END;
    MSG_ASSERT(false, "The cell required does not exist")
    return {};
}

void
LteUe::DlConnect(CellsList_t cl, DlConn dlc) {
    BEG;
    INFO("Connecting user with ", dlc.m_rbs, " RBs ", dlc.m_traffic.GetKbps(), " Kbps");
    m_dlCells = cl;
    m_dlConn = dlc;
    END;
}

void
LteUe::UlConnect(CellsList_t cl, UlConn ulc) {
    BEG;
    m_ulCells = cl;
    m_ulConn = ulc;
    INFO("Connecting user with ", ulc.m_rbs, " RBs and ", ulc.m_power.GetDbm(), " dBm");
    END;
}

void
LteUe::UlSetPower(Power pow) {
    BEG;
    END;
}

LteUe::CellsList_t const&
LteUe::ReadDlConnList(void) const {
    BEG END;
    return m_dlCells;
}

LteUe::DlConn const&
LteUe::ReadDlConnInfo(void) const {
    BEG END;
    return m_dlConn;
}

LteUe::CellsList_t const&
LteUe::ReadPrevDlConnList(void) const {
    BEG END;
    return m_prevDlCells;
}

LteUe::CellsList_t const&
LteUe::ReadUlConnList(void) const {
    BEG END;
    return m_ulCells;
}

LteUe::UlConn const&
LteUe::ReadUlConnInfo(void) const {
    BEG END;
    return m_ulConn;
}

LteUe::CellsList_t const&
LteUe::ReadPrevUlConnList(void) const {
    BEG END;
    return m_prevUlCells;
}

void
LteUe::CallUp(void) {
    BEG;
    m_prevDlCells = m_dlCells;
    m_prevUlCells = m_ulCells;

    m_sensedDlPower.clear();
    m_ulEstimates.clear();
    m_dlCells.clear();
    m_ulCells.clear();
    m_dlConn = DlConn();
    m_ulConn = UlConn();
    END;
}