#ifndef SINR_H
#define SINR_H

#include "Power.h"
#include "Bandwidth.h"
#include <functional>
#include <utility>

constexpr long double K_b = 1.38064852e-23;
constexpr double T = 293.15; // assumed 20ºC



constexpr double
NoisePowerWatt(double hz)
{
    return hz*K_b*T;
}

inline double
NoisePowerWatt(Bandwidth const& bw)
{
    return bw.GetHz() * K_b*T;
}

/**
 * \ingroup core-utils
 * \brief Class to handle and calculate SINR or SIR
 * In order to add the thermal noise properly the bandwidth is to have 
 * specified
 */
class Sinr
{
public:
    
    Sinr ();
    /**
     * \brief Constructor. The user must assure that the power is referred to the 
     * set bandwidth
     * \param band --> Bandwidth (for the noise)
     * \param signal --> Received power
     * \param interference --> Interference power, Default to 0.0 Watt
     */
    explicit Sinr(Bandwidth band, Power signal, Power interference = Power(units::Watt(0.0)));

    /**
     * \brief Constructor without bandwidth, so that noise is set to 0
     * (interference limited system)
     * \param signal --> Received power
     * \param interference --> Interference power. Default to 0.0 Watt
     */
    explicit Sinr(Power signal, Power interference = Power(units::Watt(0.0)));

    Sinr(Sinr const& other);

    Sinr& operator=(Sinr const& other);

    /**
     * \brief Add more power to signal. 
     * \param pow <-- Additional power
     */
    void AddSignal(Power const& pow);
    void AddSignal(units::Watt const& pow);
    void AddSignal(units::MilliWatt const& pow);
    void AddSignal(units::dBW const& pow);
    void AddSignal(units::dBm const& pow);

    /**
     * \brief Add more interfering power. 
     * \param pow <-- Additional power
     */
    void AddInterference(Power const& pow);
    void AddInterference(units::Watt const& pow);
    void AddInterference(units::MilliWatt const& pow);
    void AddInterference(units::dBW const& pow);
    void AddInterference(units::dBm const& pow);

    double BwHz(void) const;
    double BwKhz(void) const;
    double BwMhz(void) const;

    double SignalW(void) const;
    double SignalMw(void) const;
    double SignalDbw(void) const;
    double SignalDbm(void) const;

    double InterferenceW(void) const;
    double InterferenceMw(void) const;
    double InterferenceDbw(void) const;
    double InterferenceDbm(void) const;

    /**
     * \brief Give value of the SINR either in linear or logarithmic scale
     * \return <-- Value
     */
    double SinrLin() const;
    double SinrLog() const;

private:
    Power m_signal;
    Power m_interference;
    Bandwidth m_band;
    Power m_noise;
};

#endif /* SINR_H */

