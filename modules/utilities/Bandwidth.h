#ifndef BANDWIDTH_H
#define BANDWIDTH_H

#include "CommonUnits.h"


/**
 * \ingroup core-utils
 * \brief Class to store bandwidths in Hz, KHz, or MHz.
 * 
 */
class Bandwidth
{
public:
    /**
     * \brief Constructor from any (defined) unit
     */
    explicit Bandwidth (units::MHz v );
    explicit Bandwidth (units::kHz v );
    explicit Bandwidth (units::Hz v );
    explicit Bandwidth (units::dBMHz v );
    explicit Bandwidth (units::dBkHz v );
    explicit Bandwidth (units::dBHz v );
    
    /**
     * \brief Relational and comparison operators
     */
    bool operator<(Bandwidth const& p) const;
    bool operator<(units::Hz const& p) const;
    bool operator<(units::kHz const& p) const;
    bool operator<(units::MHz const& p) const;
    bool operator<(units::dBHz const& p) const;
    bool operator<(units::dBkHz const& p) const;
    bool operator<(units::dBMHz const& p) const;
    bool operator<=(Bandwidth const& p) const;
    bool operator<=(units::Hz const& p) const;
    bool operator<=(units::kHz const& p) const;
    bool operator<=(units::MHz const& p) const;
    bool operator<=(units::dBHz const& p) const;
    bool operator<=(units::dBkHz const& p) const;
    bool operator<=(units::dBMHz const& p) const;
    bool operator>(Bandwidth const& p) const;
    bool operator>(units::Hz const& p) const;
    bool operator>(units::kHz const& p) const;
    bool operator>(units::MHz const& p) const;
    bool operator>(units::dBHz const& p) const;
    bool operator>(units::dBkHz const& p) const;
    bool operator>(units::dBMHz const& p) const;
    bool operator>=(Bandwidth const& p) const;
    bool operator>=(units::Hz const& p) const;
    bool operator>=(units::kHz const& p) const;
    bool operator>=(units::MHz const& p) const;
    bool operator>=(units::dBHz const& p) const;
    bool operator>=(units::dBkHz const& p) const;
    bool operator>=(units::dBMHz const& p) const;
    bool operator==(Bandwidth const& p) const;
    bool operator==(units::Hz const& p) const;
    bool operator==(units::kHz const& p) const;
    bool operator==(units::MHz const& p) const;
    bool operator==(units::dBHz const& p) const;
    bool operator==(units::dBkHz const& p) const;
    bool operator==(units::dBMHz const& p) const;
    bool operator!=(Bandwidth const& p) const;
    bool operator!=(units::Hz const& p) const;
    bool operator!=(units::kHz const& p) const;
    bool operator!=(units::MHz const& p) const;
    bool operator!=(units::dBHz const& p) const;
    bool operator!=(units::dBkHz const& p) const;
    bool operator!=(units::dBMHz const& p) const;
    
    /**
     * \brief Read value in different units (linear and logarithmic)
     * \return <-- Value
     */
    double GetMhz ( void ) const;
    double GetKhz ( void ) const;
    double GetHz ( void ) const;
    double GetDbMhz ( void ) const;
    double GetDbKhz ( void ) const;
    double GetDbHz ( void ) const;
    
private:
    units::kHz m_khz;
};

#endif /* BANDWIDTH_H */

