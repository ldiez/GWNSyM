#ifndef POSITION_H
#define	POSITION_H

#include <cassert>
#include <tuple>
#include <vector>
#include <cmath>

#include "CoreUtils.h"
#include "Distance.h"

/**
 * \brief 3D position structure. The position can be converted to and contrudted from
 * std::vector or std::tuple of double or float
 */
struct Position
{
    /**
     * \brief The point defining a position MUST be in meters regarding the
     * current system coordinates
     */
    Position ( double x = 0.0f, double y = 0.0f, double z = 0.0f );

    Position ( Position const& pos );

    Position ( std::vector<double> const& dv );

    Position ( std::vector<float> const& dv );

    Position ( std::tuple<double, double, double> const& dt );

    Position ( std::tuple<float, float, float> const& dt );

    Position ( std::tuple<double, double> const& dt );

    Position ( std::tuple<float, float> const& dt );

    double GetX ( void ) const;
    double GetY ( void ) const;
    double GetZ ( void ) const;

    void SetX ( double );
    void SetY ( double );
    void SetZ ( double );

    /**
     * Implicit conversion to other type
     */
    operator std::vector<double> ( ) const;

    operator std::vector<float> ( ) const;

    operator std::tuple<double, double, double> ( ) const;

    operator std::tuple<float, float, float> ( ) const;

    Position& operator= ( std::vector<double> const& dv );

    Position& operator= ( std::vector<float> const& dv );

    Position& operator= ( std::tuple<double, double, double> const& dt );

    Position& operator= ( std::tuple<float, float, float> const& dt );

    Position& operator= ( std::tuple<double, double> const& dt );

    Position& operator= ( std::tuple<float, float> const& dt );

    /**
     * Distances
     */
    Distance GetPlanarDistance ( Position const& other );

    Distance Get3dDistance ( Position const& other );

    Distance GetXDeltaAbs ( Position const& other );

    Distance GetYDeltaAbs ( Position const& other );

    Distance GetZDeltaAbs ( Position const& other );

    Distance GetXDelta ( Position const& other );

    Distance GetYDelta ( Position const& other );

    Distance GetZDelta ( Position const& other );

    friend Distance GetPlanarDistance ( Position const& lhs, Position const& rhs );
    
    friend Distance Get3dDistance ( Position const& lhs, Position const& rhs );

    friend Distance GetXDeltaAbs ( Position const& lhs, Position const& rhs );
    
    friend Distance GetYDeltaAbs ( Position const& lhs, Position const& rhs );

    friend Distance GetZDeltaAbs ( Position const& lhs, Position const& rhs );

    friend Distance GetXDelta ( Position const& lhs, Position const& rhs );

    friend Distance GetYDelta ( Position const& lhs, Position const& rhs );
    
    friend Distance GetZDelta ( Position const& lhs, Position const& rhs );
    
    friend std::ostream& operator << ( std::ostream& o, Position const& pos );

private:
    double m_x;
    double m_y;
    double m_z;
};

inline Distance
GetPlanarDistance ( Position const& lhs, Position const& rhs )
{
    return Distance(units::m(
            std::sqrt(std::pow(std::fabs(lhs.m_x - rhs.m_x), 2.0) +
                     std::pow(std::fabs(lhs.m_y - rhs.m_y), 2.0))
            ));
}

inline Distance
Get3dDistance ( Position const& lhs, Position const& rhs )
{
    return Distance(units::m(std::sqrt(std::pow(std::fabs(lhs.m_x - rhs.m_x), 2.0) +
                     std::pow(std::fabs(lhs.m_y - rhs.m_y), 2.0) +
                     std::pow(std::fabs(lhs.m_z - rhs.m_z), 2.0))));
}

inline Distance 
GetXDeltaAbs ( Position const& lhs, Position const& rhs )
{
    return Distance(units::m(std::fabs(lhs.m_x - rhs.m_x)));
}

inline Distance 
GetYDeltaAbs ( Position const& lhs, Position const& rhs )
{
    return Distance(units::m(std::fabs(lhs.m_y - rhs.m_y)));
}

inline Distance 
GetZDeltaAbs ( Position const& lhs, Position const& rhs )
{
    return Distance(units::m(std::fabs(lhs.m_z - rhs.m_z)));
}

inline Distance 
GetXDelta ( Position const& lhs, Position const& rhs )
{
    return Distance(units::m(lhs.m_x - rhs.m_x ));
}

inline Distance 
GetYDelta ( Position const& lhs, Position const& rhs )
{
    return Distance(units::m(lhs.m_y - rhs.m_y ));
}

inline Distance 
GetZDelta ( Position const& lhs, Position const& rhs )
{
    return Distance(units::m(lhs.m_z - rhs.m_z ));
}

inline std::ostream& operator << ( std::ostream& o, Position const& pos )
{
    o << "[" <<pos.m_x << ", " << pos.m_y << ", " << pos.m_z << "]";
    return o;
}

#endif	/* POSITION_H */