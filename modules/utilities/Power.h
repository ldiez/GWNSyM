#ifndef POWER_H
#define POWER_H

#include "CommonUnits.h"

/**
 * \ingorup core-utils
 * \brief Class to handle power levels:
 * - addition
 * - subtraction
 * - attenuation
 * - amplification
 * - units conversion
 */
class Power
{
public:

    /**
     * \brief Construct Power by power value
     */
    explicit Power(units::Watt w = units::Watt(0.0));
    explicit Power(units::MilliWatt mw);
    explicit Power(units::dBW d);
    explicit Power(units::dBm d);
    Power(Power const& other);

    /**
     * \brief Copy assignment operators
     */
    Power& operator=(Power const& p);
    Power& operator=(units::Watt const& p);
    Power& operator=(units::MilliWatt const& p);
    Power& operator=(units::dBW const& p);
    Power& operator=(units::dBm const& p);

    /**
     * \brief Add/Subtract power 
     */
    Power operator+(Power const& p) const;
    void operator+=(Power const& p);
    Power operator-(Power const& p) const;
    void operator-=(Power const& p);
    Power operator+(units::Watt const& w) const;
    void operator+=(units::Watt const& w);
    Power operator+(units::MilliWatt const& mw) const;
    void operator+=(units::MilliWatt const& mw);
    Power operator+(units::dBW const& dbw) const;
    void operator+=(units::dBW const& dbw);
    Power operator+(units::dBm const& dbm) const;
    void operator+=(units::dBm const& dbm);
    Power operator-(units::Watt const& w) const;
    void operator-=(units::Watt const& w);
    Power operator-(units::MilliWatt const& mw) const;
    void operator-=(units::MilliWatt const& mw);
    Power operator-(units::dBW const& dbw) const;
    void operator-=(units::dBW const& dbw);
    Power operator-(units::dBm const& dbm) const;
    void operator-=(units::dBm const& dbm);



    /**
     * \brief Relational and comparison operators
     */
    bool operator<(Power const& p) const;
    bool operator<(units::Watt const& p) const;
    bool operator<(units::MilliWatt const& p) const;
    bool operator<(units::dBW const& p) const;
    bool operator<(units::dBm const& p) const;
    bool operator<=(Power const& p) const;
    bool operator<=(units::Watt const& p) const;
    bool operator<=(units::MilliWatt const& p) const;
    bool operator<=(units::dBW const& p) const;
    bool operator<=(units::dBm const& p) const;
    bool operator>(Power const& p) const;
    bool operator>(units::Watt const& p) const;
    bool operator>(units::MilliWatt const& p) const;
    bool operator>(units::dBW const& p) const;
    bool operator>(units::dBm const& p) const;
    bool operator>=(Power const& p) const;
    bool operator>=(units::Watt const& p) const;
    bool operator>=(units::MilliWatt const& p) const;
    bool operator>=(units::dBW const& p) const;
    bool operator>=(units::dBm const& p) const;
    bool operator==(Power const& p) const;
    bool operator==(units::Watt const& p) const;
    bool operator==(units::MilliWatt const& p) const;
    bool operator==(units::dBW const& p) const;
    bool operator==(units::dBm const& p) const;
    bool operator!=(Power const& p) const;
    bool operator!=(units::Watt const& p) const;
    bool operator!=(units::MilliWatt const& p) const;
    bool operator!=(units::dBW const& p) const;
    bool operator!=(units::dBm const& p) const;

    /**
     * \brief Amplify/Attenuate power either by dB or lineal scale
     */
    void Att(double a);
    void Att(units::dB const& d);
    void Amp(double a);
    void Amp(units::dB const& d);

    /**
     * \brief Get raw data in a given unit
     * \return <-- Value
     */
    double GetWatt(void) const;
    double GetMilliWatt(void) const;
    double GetDbw(void) const;
    double GetDbm(void) const;

private:
//    Power() = delete;
    units::Watt m_w;
};





#endif /* POWER_H */

