#include "LteUeConf.h"
#include "Log.h"
#include "Configurer.h"
#include "ConfigXml.h"

LOG_REGISTER_MODULE ( "LteUeConf" );

LteUeConf::LteUeConf ( std::string&& filename ) 
: m_rxGain ( 0.0 )
, m_noiseFigure ( 0.0 )
, m_ulSinrTh(0.0)
{
    ConfigXml conf_(filename);
    m_rxGain = conf_.Get<double>("USER.LTE_UE.RX_GAIN");
    m_noiseFigure = conf_.Get<double>("USER.LTE_UE.NOISE_FIGURE");
    m_ulSinrTh = conf_.Get<double>("USER.LTE_UE.UL_SINR_TH");
    
    INFO ("LTE UE is configure with: ")
    INFO ("    Rx gain of ", m_rxGain.RawVal(), "dB");
    INFO ("    NF of ", m_noiseFigure.RawVal(), "dB");
}

units::dB
LteUeConf::ReadNoiseFigure ( void ) const
{
    BEG END;
    return m_noiseFigure;
}

units::dB
LteUeConf::ReadRxGain ( void ) const
{
    BEG END;
    return m_rxGain;
}

units::dB
LteUeConf::UlSinrTh ( void ) const
{
    BEG END;
    return m_ulSinrTh;
}