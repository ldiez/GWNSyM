#include "LteClusterConf.h"
#include "Log.h"
#include "ConfigXml.h"

LOG_REGISTER_MODULE("LteClusterConf");

LteClusterConf::LteClusterConf(std::string const& path, std::string const& name)
: m_cName(name)
{
    BEG;
    ConfigXml cxml_(path);
    m_macroNumber = cxml_.Get<std::uint32_t>(name + ".MACRO_NUMBER");
    m_picoNumber = cxml_.Get<std::uint32_t>(name + ".PICO_NUMBER");
    END;
}

gnsm::InnerItemsCounterList_t
LteClusterConf::ReadInnerConf(void) const
{
    BEG END;

    return
    {
        {
            "MACRO", m_macroNumber
        },
        {
            "PICO", m_picoNumber
        }
    };
}

std::string
LteClusterConf::ReadClusterName(void) const
{
    BEG END;
    return m_cName;
}