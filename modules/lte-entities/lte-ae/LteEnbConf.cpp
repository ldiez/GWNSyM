#include "LteEnbConf.h"
#include "ConfigXml.h"

#include "Log.h"

LOG_REGISTER_MODULE("LteEnbConf");

LteEnbConf::LteEnbConf(EnbType enbType, std::string&& path)
: m_angle(0.0)
, m_numCells(0u)
, m_enbType(enbType)
, m_freq(0.0)
, m_cellSetName()
{
    BEG;

    ConfigXml conf_(path);
    auto strEnbType_ = ToStr(m_enbType);

    m_numCells = conf_.Get<std::uint32_t>(strEnbType_ + ".CELLS_NUM", 1u);
    m_freq = conf_.Get<double>(strEnbType_ + ".FREQ_GHZ");
    m_angle = conf_.Get<double>(strEnbType_ + ".SECTOR_ANGLE", 0.0);
    m_cellSetName = strEnbType_+"_CELL";
    
    INFO ("eNB ", strEnbType_, " is configured with");
    INFO ("    ", m_numCells, " cells");
    INFO ("    operating frequency ", m_freq.RawVal(), " GHz");
    INFO ("    sector angle ", m_angle, "º");
    INFO ("    cell set name ", m_cellSetName);
    END;
}

gnsm::InnerItemsCounterList_t
LteEnbConf::ReadInnerConf(void) const
{
    BEG END;
    return
    {
        {
            ToStr(m_enbType)+"_CELL", m_numCells
        }
    };
}

double
LteEnbConf::GetSectorizationAngle(void) const
{
    BEG END;
    return m_angle;
}

units::GHz
LteEnbConf::GetFrequency(void) const
{
    BEG END;
    return m_freq;

}

EnbType
LteEnbConf::GetType(void) const
{
    BEG END;
    return m_enbType;
}

std::string 
LteEnbConf::GetCellSetName ( void ) const
{
    BEG END;
    return m_cellSetName;
}