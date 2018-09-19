#include "LteUeConf.h"
#include "Log.h"
#include "Configurer.h"
#include "ConfigXml.h"

LOG_REGISTER_MODULE("LteUeConf");

LteUeConf::LteUeConf(std::string&& filename)
: m_rxGain(0.0)
, m_txGain(0.0)
, m_noiseFigure(0.0)
, m_ulSinrTh(0.0)
, m_biasMacro(0.0)
, m_biasMicro(0.0)
, m_biasPico(0.0)
{
    ConfigXml conf_(filename);
    m_rxGain = conf_.Get<double>("USER.LTE_UE.RX_GAIN");
    m_txGain = conf_.Get<double>("USER.LTE_UE.TX_GAIN");
    m_noiseFigure = conf_.Get<double>("USER.LTE_UE.NOISE_FIGURE");
    m_ulSinrTh = conf_.Get<double>("USER.LTE_UE.UL_SINR_TH");
    m_pMax = units::MilliWatt(conf_.Get<double>("USER.LTE_UE.P_MAX"));
    m_alpha = conf_.Get<double>("USER.LTE_UE.UL_ALPHA");

    m_biasMacro = conf_.Get<double>("USER.CRE.BIAS_MACRO", 0);
    m_biasMicro = conf_.Get<double>("USER.CRE.BIAS_MICRO", 0);
    m_biasPico = conf_.Get<double>("USER.CRE.BIAS_PICO", 0);

    INFO("LTE UE is configure with: ")
    INFO("    Rx gain of ", m_rxGain.RawVal(), "dB");
    INFO("    NF of ", m_noiseFigure.RawVal(), "dB");
    INFO("    bias macro ", m_biasMacro.RawVal(), "dB");
    INFO("    bias micro ", m_biasMicro.RawVal(), "dB");
    INFO("    bias pico ", m_biasPico.RawVal(), "dB");
}

units::dB
LteUeConf::GetNoiseFigure(void) const
{
    BEGEND;
    return m_noiseFigure;
}

units::dB
LteUeConf::GetRxGain(void) const
{
    BEGEND;
    return m_rxGain;
}

units::dB
LteUeConf::GetTxGain(void) const
{
    BEGEND;
    return m_txGain;
}

units::dB
LteUeConf::UlSinrTh(void) const
{
    BEGEND;
    return m_ulSinrTh;
}

Power
LteUeConf::GetPmax(void) const
{
    BEGEND;
    return m_pMax;
}

double
LteUeConf::GetAlpha(void) const
{
    BEGEND;
    return m_alpha;
}

units::dB
LteUeConf::GetBiasMacro(void) const
{
    BEGEND;
    return m_biasMacro;
}

units::dB
LteUeConf::GetBiasMicro(void) const
{
    BEGEND;
    return m_biasMicro;
}

units::dB
LteUeConf::GetBiasPico(void) const
{
    BEGEND;
    return m_biasPico;
}