#include "Sinr.h"
#include "Log.h"

using namespace units;

LOG_REGISTER_MODULE("Sinr")

Sinr::Sinr()
: m_signal(units::Watt(0.0))
, m_interference(units::Watt(0.0))
, m_band(kHz(0.0))
, m_noise(units::Watt(0.0)) {
    BEG;
    INFO("Signal power ", m_signal.GetDbm(), " dBm");
    INFO("Noise power ", m_noise.GetDbm(), " dBm");
    INFO("Interference power ", m_interference.GetDbm(), " dBm");
    END;
}

Sinr::Sinr(Bandwidth band, Power rx, Power interference)
: m_signal(rx)
, m_interference(interference)
, m_band(band)
, m_noise(Watt(NoisePowerWatt(m_band))) {
    BEG;
    INFO("Signal power ", m_signal.GetDbm(), " dBm");
    INFO("Noise power ", m_noise.GetDbm(), " dBm");
    INFO("Interference power ", m_interference.GetDbm(), " dBm");
    END;
}

Sinr::Sinr(Bandwidth band)
: m_signal(units::Watt(0.0))
, m_interference(units::Watt(0.0))
, m_band(band)
, m_noise(Watt(NoisePowerWatt(m_band))) {
    BEG;
    INFO("Signal power ", m_signal.GetDbm(), " dBm");
    INFO("Noise power ", m_noise.GetDbm(), " dBm");
    INFO("Interference power ", m_interference.GetDbm(), " dBm");
    END;
}

Sinr::Sinr(Power rx, Power interference)
: m_signal(rx)
, m_interference(interference)
, m_band(kHz(0.0))
, m_noise(Watt(0.0)) {
    BEG;
    INFO("Signal power ", m_signal.GetDbm(), " dBm");
    INFO("Noise power ", m_noise.GetDbm(), " dBm");
    INFO("Interference power ", m_interference.GetDbm(), " dBm");
    END;
}

Sinr::Sinr(Sinr const& other)
: m_signal(other.m_signal)
, m_interference(other.m_interference)
, m_band(other.m_band)
, m_noise(other.m_noise) {
    BEG;
    INFO("Signal power ", m_signal.GetDbm(), " dBm");
    INFO("Noise power ", m_noise.GetDbm(), " dBm");
    INFO("Interference power ", m_interference.GetDbm(), " dBm");
    END;
}

Sinr&
        Sinr::operator=(Sinr const& other) {
    BEG;
    m_signal = other.m_signal;
    m_interference = other.m_interference;
    m_band = other.m_band;
    m_noise = other.m_noise;
    INFO("Signal power ", m_signal.GetDbm(), " dBm");
    INFO("Noise power ", m_noise.GetDbm(), " dBm");
    INFO("Interference power ", m_interference.GetDbm(), " dBm");
    END;
    return *this;
}

void
Sinr::AddSignal(Power const& pow) {
    BEG;
    m_signal += pow;
    END;
}

void
Sinr::AddSignal(Watt const& pow) {
    BEG;
    m_signal += pow;
    END;
}

void
Sinr::AddSignal(MilliWatt const& pow) {
    BEG;
    m_signal += pow;
    END;
}

void
Sinr::AddSignal(dBW const& pow) {
    BEG;
    m_signal += pow;
    END;
}

void
Sinr::AddSignal(dBm const& pow) {
    BEG;
    m_signal += pow;
    END;
}

void
Sinr::AddInterference(Power const& pow) {
    BEG;
    m_interference += pow;
    END;
}

void
Sinr::AddInterference(Watt const& pow) {
    BEG;
    m_interference += pow;
    END;
}

void
Sinr::AddInterference(MilliWatt const& pow) {
    BEG;
    m_interference += pow;
    END;
}

void
Sinr::AddInterference(dBW const& pow) {
    BEG;
    m_interference += pow;
    END;
}

void
Sinr::AddInterference(dBm const& pow) {
    BEG;
    m_interference += pow;
    END;
}

double
Sinr::BwHz(void) const {
    BEGEND;
    return m_band.GetHz();
}

double
Sinr::BwKhz(void) const {
    BEGEND;
    return m_band.GetKhz();
}

double
Sinr::BwMhz(void) const {
    BEGEND;
    return m_band.GetDbMhz();
}

double
Sinr::SignalW(void) const {
    BEGEND;
    return m_signal.GetWatt();
}

double
Sinr::SignalMw(void) const {
    BEGEND;
    return m_signal.GetMilliWatt();
}

double
Sinr::SignalDbw(void) const {
    BEGEND;
    return m_signal.GetDbw();
}

double
Sinr::SignalDbm(void) const {
    BEGEND;
    return m_signal.GetDbm();
}

double
Sinr::InterferenceW(void) const {
    BEGEND;
    return m_interference.GetWatt();
}

double
Sinr::InterferenceMw(void) const {
    BEGEND;
    return m_interference.GetMilliWatt();
}

double
Sinr::InterferenceDbw(void) const {
    BEGEND;
    return m_interference.GetDbw();
}

double
Sinr::InterferenceDbm(void) const {
    BEGEND;
    return m_interference.GetDbm();
}

double Sinr::NoiseW(void) const {
    BEGEND;
    return m_noise.GetWatt();
}

double Sinr::NoiseMw(void) const {
    BEGEND;
    return m_noise.GetMilliWatt();
}

double Sinr::NoiseDbw(void) const {
    BEGEND;
    return m_noise.GetDbw();
}

double Sinr::NoiseDbm(void) const {
    BEGEND;
    return m_noise.GetDbm();
}

double
Sinr::SinrLin() const {
    BEGEND;
    if (m_signal.GetWatt() == 0.0) // division by 0
    {
        return 0.0;
    }
    return m_signal.GetWatt() / (m_interference.GetWatt() + m_noise.GetWatt());
}

double
Sinr::SinrLog() const {
    BEGEND;
    return Lin2log(SinrLin());
}