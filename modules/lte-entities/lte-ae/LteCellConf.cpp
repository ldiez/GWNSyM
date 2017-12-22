#include "LteCellConf.h"
#include "Log.h"
#include "Configurer.h"
#include <cmath>
#include <complex>
#include "ConfigXml.h"
#include "PowerParser.h"


LOG_REGISTER_MODULE("LteCellConf");

LteCellConf::LteCellConf(EnbType enbType, std::string const& path)
: m_type(enbType)
, m_txPower(units::Watt(0.0))
, m_txPowerPerRb(units::Watt(0.0))
, m_capacity(0.0)
, m_txGain(0.0)
, m_nf(0.0)
{
    BEG;
    ConfigXml conf_(path);

    auto strEnbType_ = ToStr(m_type);
    m_capacity = conf_.Get<double>(strEnbType_ + ".CELL.CAPACITY");
    m_txPower = ParsePow(conf_.GetAttr<>(strEnbType_ + ".CELL.TX_POWER", "units"),
                         conf_.Get<double>(strEnbType_ + ".CELL.TX_POWER"));
    m_txGain = conf_.Get<double>(strEnbType_ + ".CELL.TX_GAIN");
    m_txPowerPerRb = units::Watt(m_txPower.GetWatt() / m_capacity);
    m_nf = conf_.Get<double>(strEnbType_ + ".CELL.NF");

    INFO("Cell ", strEnbType_, " is configured with");
    INFO("    capacity of ", m_capacity, " resource blocks");
    INFO("    tx power of ", m_txPower.GetDbm(), " dBm");
    INFO("    tx gain of ", m_txGain.RawVal(), " dB");
    INFO("    tx power per RB ", m_txPowerPerRb.GetDbm(), " dBm");
    END;
}

Power
LteCellConf::GetTxPower(void) const
{
    BEG END;
    return m_txPower;
}

Power
LteCellConf::GetTxpowerPerRb(void) const
{
    BEG END;
    return m_txPowerPerRb;
}

double
LteCellConf::GetCapacity(void) const
{
    BEG END;
    return m_capacity;
}

units::dB
LteCellConf::GetTxGain(void) const
{
    BEG END;
    return m_txGain;
}

EnbType
LteCellConf::GetType(void) const
{
    BEG END;
    return m_type;
}

std::string
LteCellConf::GetTypeName(void) const
{
    BEG END;
    return ToStr(m_type);
}

units::dB 
LteCellConf::GetNoiseFigure ( void ) const
{
    BEG END;
    return m_nf;
}