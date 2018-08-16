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
, m_prevUlInterf(units::Watt(0.0))
, m_id(id) {
    BEG END;
}

void
LteUe::SetConfiguration(LteUeConf const& conf) {
    BEG END;
    m_conf = &conf;
}

LteUeConf const&
LteUe::GetConfiguration(void) {
    BEG END;
    return *m_conf;
}

void
LteUe::AddCellInfo(gnsm::Id_t enbId, gnsm::Ptr_t<LteCell> cell, Power rsrp, units::dB ulPl, EnbType type) {
    BEG;
    INFO("Adding cell [", cell, "] of type ", type, " with Rx power ",
            rsrp.GetMilliWatt(), " mW");
    m_orderedDl = false;
    m_orderedUl = false;
    m_sensedDlPower.push_back({enbId, cell, rsrp, type});
    m_ulEstimates.push_back({enbId, cell, ulPl, type});
    END;
}

LteUe::SensedValues_t const&
LteUe::GetOrderedCellsDl(void) {
    BEG;
    if (!m_orderedDl) {
        std::sort(m_sensedDlPower.begin(), m_sensedDlPower.end(), SensedSort);
        m_orderedDl = true;
    }
    END;
    return m_sensedDlPower;
}

LteUe::CellScan const&
LteUe::GetOrderedCellDl(std::uint32_t pos) {
    BEG;
    MSG_ASSERT(m_sensedDlPower.size() > pos, "Bad cell index")
    if (!m_orderedDl) {
        std::sort(m_sensedDlPower.begin(), m_sensedDlPower.end(), SensedSort);
        m_orderedDl = true;
    }
    END;
    return m_sensedDlPower.at(pos);
}

LteUe::CellScan
LteUe::GetCellDl(gnsm::Ptr_t<LteCell> const& c) {
    BEG;
    for (auto& item_ : m_sensedDlPower) {
        if (item_.m_cell == c) {
            return item_;
        }
    }
    END;
    MSG_ASSERT(false, "Required cell does not exist")
    return {};
}

LteUe::UlEstimate_t const&
LteUe::GetOrderedCellsUl(void) {
    BEG;
    if (!m_orderedUl) {
        std::sort(m_ulEstimates.begin(), m_ulEstimates.end(), EstimateSort);
        m_orderedUl = true;
    }
    END;
    return m_ulEstimates;
}

LteUe::UlLosses const&
LteUe::GetOrderedCellUl(std::uint32_t pos) {
    BEG;
    MSG_ASSERT(m_ulEstimates.size() > pos, "Bad cell index")
    if (!m_orderedUl) {
        std::sort(m_ulEstimates.begin(), m_ulEstimates.end(), EstimateSort);
        m_orderedUl = true;
    }
    END;
    return m_ulEstimates.at(pos);
}

LteUe::UlLosses
LteUe::GetCellUl(const gnsm::Ptr_t<LteCell>& c) {
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
LteUe::UlConnect(CellsList_t cl, double nrbs) {
    BEG;
    m_ulCells = cl;
    m_ulConn.m_rbs = nrbs;
    END;
}

void
LteUe::UlConnect(CellsList_t cl, double nrbs, Power pow) {
    BEG;
    m_ulCells = cl;
    m_ulConn.m_rbs = nrbs;
    m_ulConn.m_power = pow;
    END;
}

void
LteUe::UlSetPower(Power pow) {
    BEG;
    m_ulConn.m_power = pow;
    END;
}

void
LteUe::UlSetSinr(Sinr sinr) {
    BEG;
    m_ulConn.m_sinr = sinr;
    END;
}

LteUe::CellsList_t const&
LteUe::GetDlConnList(void) const {
    BEG END;
    return m_dlCells;
}

LteUe::DlConn const&
LteUe::GetDlConnInfo(void) const {
    BEG END;
    return m_dlConn;
}

LteUe::CellsList_t const&
LteUe::GetPrevDlConnList(void) const {
    BEG END;
    return m_prevDlCells;
}

LteUe::CellsList_t const&
LteUe::GetUlConnList(void) const {
    BEG END;
    return m_ulCells;
}

LteUe::UlConn const&
LteUe::GetUlConnInfo(void) const {
    BEG END;
    return m_ulConn;
}

LteUe::CellsList_t const&
LteUe::GetPrevUlConnList(void) const {
    BEG END;
    return m_prevUlCells;
}

Power const&
LteUe::GetPrevUlInterf(void) const {
    BEGEND;
    return m_prevUlInterf;
}

LteUe::UlConn const&
LteUe::GetPrevUlConn(void) const {
    BEGEND;
    return m_prevUlConn;
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
    m_prevUlInterf = m_ulConn.m_sinr.Interference();
    m_prevUlConn = m_ulConn;
    m_ulConn = UlConn();
    END;
}

Sinr
LoadBasedSinr(LteUe::SensedValues_t const& cells, LteUe::CellScan cellInfo, double load) {
    MSG_ASSERT(load >= 0.0 and load <= 1.0, "Load values has to be relative load "
            "within [0,1]");

    auto servEnb = cellInfo.m_cell->GetEnb();
    Sinr s(Bandwidth(LTE::RbBw_s), cellInfo.m_rsrp);
    for (auto& item : cells) {
        if (item.m_cell == cellInfo.m_cell or servEnb == item.m_cell->GetEnb()) {
            continue;
        }
        auto inter = item.m_rsrp;
        s.AddInterference(inter.Amp(load));
    }
    INFO("SINR ", s.SinrLog(), " dB");
    return s;
}

bool
isConnected(gnsm::Ptr_t<LteUe> ue) {
    auto servCells = ue->GetUlConnList();
    return (servCells.size() >= 1);
}