#include "LteCell.h"
#include "Log.h"

LOG_REGISTER_MODULE("LteCell")

LteCell::LteCell(gnsm::Id_t id)
: m_id(id)
, m_conf(nullptr)
, m_azimut(0.0)
, m_freeResources(0.0)
, m_nConn(0u)
{
}

void
LteCell::SetConfiguration(LteCellConf const& conf)
{
    BEG;
    m_conf = &conf;
    m_freeResources = m_conf->GetCapacity();
    END;
}

gnsm::Id_t const&
LteCell::ReadId(void) const
{
    BEG END;
    return m_id;
}

LteCellConf const&
LteCell::ReadConfig(void) const
{
    BEG END;
    return *m_conf;
}

bool
LteCell::AddUser(double cap)
{
    BEG;
    if (m_freeResources >= cap)
    {
        ++m_nConn;
        m_freeResources -= cap;
        INFO(this, " Adding connection with capacity ", cap, " Free resources ", m_freeResources,
             " after ", m_nConn, " connections");
        return true;
    }
    END;
    return false;
}

void
LteCell::SetAzimut(double azimut)
{
    BEG;
    m_azimut = azimut;
    END;
}

double
LteCell::GetAzimut(void) const
{
    BEG END;
    return m_azimut;
}

double
LteCell::GetFreeResources(void) const
{
    BEG END;
    return m_freeResources;
}

double
LteCell::GetLoad(void) const
{
    BEG END;
    return (1.0 - m_freeResources / m_conf->GetCapacity());
}

void
LteCell::CallUp(void)
{
    BEG;
    m_freeResources = m_conf->GetCapacity();
    m_nConn = 0u;
    END;
}