#include "LteUeConf.h"
#include "Log.h"
#include "Configurer.h"
#include "ConfigXml.h"

LOG_REGISTER_MODULE("LteUeConf");

LteUeConf::LteUeConf(std::string&& filename)
: m_rxGain(0.0)
, m_txGain(0.0)
, m_noiseFigure(0.0)
, m_ulSinrTh(0.0) {
    ConfigXml conf_(filename);
    m_rxGain = conf_.Get<double>("USER.LTE_UE.RX_GAIN");
    m_txGain = conf_.Get<double>("USER.LTE_UE.TX_GAIN");
    m_noiseFigure = conf_.Get<double>("USER.LTE_UE.NOISE_FIGURE");
    m_ulSinrTh = conf_.Get<double>("USER.LTE_UE.UL_SINR_TH");
    m_pMax = units::MilliWatt(conf_.Get<double>("USER.LTE_UE.P_MAX"));
    m_alpha = conf_.Get<double>("USER.LTE_UE.UL_ALPHA");
    INFO("LTE UE is configure with: ")
    INFO("    Rx gain of ", m_rxGain.RawVal(), "dB");
    INFO("    NF of ", m_noiseFigure.RawVal(), "dB");
}

units::dB
LteUeConf::GetNoiseFigure(void) const {
    BEGEND;
    return m_noiseFigure;
}

units::dB
LteUeConf::GetRxGain(void) const {
    BEGEND;
    return m_rxGain;
}

units::dB
LteUeConf::GetTxGain(void) const {
    BEGEND;
    return m_txGain;
}

units::dB
LteUeConf::UlSinrTh(void) const {
    BEGEND;
    return m_ulSinrTh;
}

Power
LteUeConf::GetPmax(void) const {
    BEGEND;
    return m_pMax;
}

double
LteUeConf::GetAlpha(void) const {
    BEGEND;
    return m_alpha;
}