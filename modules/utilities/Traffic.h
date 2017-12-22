#ifndef TRAFFIC_H
#define TRAFFIC_H

#include "CommonUnits.h"

/**
 * \ingroup core-utils
 * \brief Class to handle traffic units
 */
class Traffic
{
public:
    Traffic ( );
    explicit Traffic(units::bps v);
    explicit Traffic(units::kbps v);
    explicit Traffic(units::Mbps v);
    
    Traffic& operator= (Traffic const& v);
    Traffic& operator= (units::bps const& v);
    Traffic& operator= (units::kbps const& v);
    Traffic& operator= (units::Mbps const& v);

    /**
     * \brief Add traffic
     */
    Traffic operator+(Traffic const& t) const;
    void operator+=(Traffic const& t);
    Traffic operator+(units::bps const& t) const;
    void operator+=(units::bps const& t);
    Traffic operator+(units::kbps const& t) const;
    void operator+=(units::kbps const& t);
    Traffic operator+(units::Mbps const& t) const;
    void operator+=(units::Mbps const& t);
    
    /**
     * \brief Subtract traffic
     */
    Traffic operator-(Traffic const& t) const;
    void operator-=(Traffic const& t);
    Traffic operator-(units::bps const& t) const;
    void operator-=(units::bps const& t);
    Traffic operator-(units::kbps const& t) const;
    void operator-=(units::kbps const& t);
    Traffic operator-(units::Mbps const& t) const;
    void operator-=(units::Mbps const& t);
    
    /**
     * \brief Relational and comparison operators
     */
    bool operator<(Traffic const& p) const;
    bool operator<(units::bps const& p) const;
    bool operator<(units::kbps const& p) const;
    bool operator<(units::Mbps const& p) const;
    bool operator<=(Traffic const& p) const;
    bool operator<=(units::bps const& p) const;
    bool operator<=(units::kbps const& p) const;
    bool operator<=(units::Mbps const& p) const;
    bool operator>(Traffic const& p) const;
    bool operator>(units::bps const& p) const;
    bool operator>(units::kbps const& p) const;
    bool operator>(units::Mbps const& p) const;
    bool operator>=(Traffic const& p) const;
    bool operator>=(units::bps const& p) const;
    bool operator>=(units::kbps const& p) const;
    bool operator>=(units::Mbps const& p) const;
    bool operator==(Traffic const& p) const;
    bool operator==(units::bps const& p) const;
    bool operator==(units::kbps const& p) const;
    bool operator==(units::Mbps const& p) const;
    bool operator!=(Traffic const& p) const;
    bool operator!=(units::bps const& p) const;
    bool operator!=(units::kbps const& p) const;
    bool operator!=(units::Mbps const& p) const;

    /**
     * \brief Get value of the traffic in different units
     * \return <-- Value
     */
    double GetBps(void) const;
    double GetKbps(void) const;
    double GetMbps(void) const;
    
    /**
     * \brief Get value in units
     */
    units::bps ToBps ();
    units::kbps ToKbps ();
    units::Mbps ToMbps ();
private:
    units::kbps m_kbps;
};

#endif /* TRAFFIC_H */