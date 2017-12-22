#include "LteCluster.h"
#include "Log.h"

LOG_REGISTER_MODULE("LteCluster");

LteCluster::LteCluster(gnsm::Id_t id)
: m_id(id)
{
    BEG END;
}

void
LteCluster::SetConfiguration(LteClusterConf const& conf)
{
    BEG;
    m_conf = &conf;
    END;
}

void
LteCluster::Aggregate(std::string const& name, gnsm::ts::Wrapper_t o)
{
    BEG;
    if (name == "MACRO")
    {
        o.Unwrap(m_macro);
    }
    else if (name == "PICO")
    {
        o.Unwrap(m_pico);
    }

    END;
}

gnsm::Ptr_t<LteEnb>
LteCluster::GetMacro(void) const
{
    BEG END;
    return m_macro;
}

gnsm::Vec_t<LteEnb>
LteCluster::GetPicos(void) const
{
    BEG END;
    return m_pico;
}

gnsm::Id_t 
LteCluster::ReadId ( void ) const
{
    BEG END;
    return m_id;
}

LteClusterConf const& 
LteCluster::ReadConf ( void ) const
{
    BEG END;
    return *m_conf;
}