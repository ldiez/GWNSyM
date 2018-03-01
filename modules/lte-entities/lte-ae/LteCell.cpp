#include "LteCell.h"
#include "LteEnb.h"
#include "Log.h"

LOG_REGISTER_MODULE("LteCell")

LteCell::LteCell(gnsm::Id_t id)
: m_enb(nullptr)
, m_id(id)
, m_conf(nullptr)
, m_azimut(0.0)
, m_dlFreeResources(0.0)
, m_ulFreeResources(0.0) {
}

void
LteCell::SetConfiguration(LteCellConf const& conf) {
    BEG;
    m_conf = &conf;
    m_dlFreeResources = m_conf->GetCapacity();
    m_ulFreeResources = m_conf->GetCapacity();
    END;
}

gnsm::Id_t const&
LteCell::GetId(void) const {
    BEG END;
    return m_id;
}

LteCellConf const&
LteCell::GetConfiguration(void) const {
    BEG END;
    return *m_conf;
}

bool
LteCell::AddDlUser(gnsm::Id_t ue, double cap) {
    BEG;
    if (m_dlFreeResources >= cap) {
        m_dlUes.push_back(ue);
        m_dlFreeResources -= cap;
        INFO(this, " Adding connection with capacity ", cap,
                " Free resources ", m_dlFreeResources,
                " after ", m_dlUes.size(), " connections");
        return true;
    }
    END;
    return false;
}

bool
LteCell::AddUlUser(gnsm::Id_t ue, double cap) {
    BEG;
    if (m_ulFreeResources >= cap) {
        m_ulUes.push_back(ue);
        m_ulFreeResources -= cap;
        INFO(this, " Adding connection with capacity ", cap,
                " Free resources ", m_ulFreeResources,
                " after ", m_ulUes.size(), " connections");
        return true;
    }
    END;
    return false;
}

void
LteCell::SetAzimut(double azimut) {
    BEG;
    m_azimut = azimut;
    END;
}

double
LteCell::GetAzimut(void) const {
    BEG END;
    return m_azimut;
}

double 
LteCell::GetDlResources(void) const {
    BEGEND;
    return m_conf->GetCapacity();
}

double 
LteCell::GetUlResources(void) const {
    BEGEND;
    return m_conf->GetCapacity();
}

double
LteCell::GetDlFreeResources(void) const {
    BEG END;
    return m_dlFreeResources;
}

double
LteCell::GetUlFreeResources(void) const {
    BEG END;
    return m_ulFreeResources;
}

double
LteCell::GetDlLoad(void) const {
    BEG END;
    return (1.0 - m_dlFreeResources / m_conf->GetCapacity());
}

double
LteCell::GetUlLoad(void) const {
    BEG END;
    return (1.0 - m_ulFreeResources / m_conf->GetCapacity());
}

void
LteCell::CallUp(void) {
    BEG;
    m_dlFreeResources = m_conf->GetCapacity();
    m_ulFreeResources = m_conf->GetCapacity();
    m_dlUes.clear();
    m_ulUes.clear();
    END;
}

void
LteCell::SetEnb(LteEnb* enbId) {
    BEG;
    m_enb = enbId;
    END;
}

LteEnb*
LteCell::GetEnb(void) {
    BEG END;
    return m_enb;
}

bool
LteCell::HasSameEnb(gnsm::Ptr_t<LteCell> other) const {
    BEGEND;
    return (other->GetEnb() == m_enb);
}