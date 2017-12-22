#include "ServConf.h"
#include "ConfigXml.h"
#include "Log.h"
#include "TrafficParser.h"

LOG_REGISTER_MODULE("ServConf");

ServConf::ServConf(ServType st, std::string const& fileName)
: m_type(st)
{
    BEG;
    ConfigXml conf_(fileName);

    auto strServType_ = ToStr(m_type);
    m_dlCapacity = ParseTraffic(conf_.GetAttr<>("SERVICES", "units"),
                                conf_.Get<double>("SERVICES." + strServType_ + ".DL"));
    m_ulCapacity = ParseTraffic(conf_.GetAttr<>("SERVICES", "units"),
                                conf_.Get<double>("SERVICES." + strServType_ + ".UL"));

    INFO("Service " + strServType_ + "configured with DL demand of "
         , m_dlCapacity.GetKbps(), "kbps and "
         "UL demand of ", m_ulCapacity.GetKbps(), "kbps");
    END;
}

Traffic
ServConf::ReadDlCapacity(void) const
{
    BEG END;
    return m_dlCapacity;
}

Traffic
ServConf::ReadUlCapacity(void) const
{
    BEG END;
    return m_ulCapacity;
}

ServType
ServConf::GetType(void) const
{
    BEG END;
    return m_type;

}

std::string
ServConf::GetTypeName(void)const
{
    BEG END;
    return ToStr(m_type);
}