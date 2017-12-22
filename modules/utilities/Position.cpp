#include "Position.h"

Position::Position ( double x, double y, double z )
: m_x ( x ), m_y ( y ), m_z ( z ) { }

Position::Position ( Position const& pos )
: m_x ( pos.m_x ), m_y ( pos.m_y ), m_z ( pos.m_z ) { }

Position::Position ( std::vector<double> const& dv )
{
    assert(dv.size() >= 2 && "Position (double) vector has not required dimensions");
    m_x = dv[0];
    m_y = dv[1];
    m_z = ( dv.size() > 2 ? dv[2] : 0.0f );
}

Position::Position ( std::vector<float> const& dv )
{
    assert(dv.size() >= 2 && "Position (float) vector has not required dimensions");
    m_x = dv[0];
    m_y = dv[1];
    m_z = ( dv.size() > 2 ? dv[2] : 0.0f );
}

Position::Position ( std::tuple<double, double, double> const& dt )
: m_x ( std::get<0>( dt ) ), m_y ( std::get<1>( dt ) ), m_z ( std::get<2>( dt ) ) { }

Position::Position ( std::tuple<float, float, float> const& dt )
: m_x ( std::get<0>( dt ) ), m_y ( std::get<1>( dt ) ), m_z ( std::get<2>( dt ) ) { }

Position::Position ( std::tuple<double, double> const& dt )
: m_x ( std::get<0>( dt ) ), m_y ( std::get<1>( dt ) ), m_z ( 0.0f ) { }

Position::Position ( std::tuple<float, float> const& dt )
: m_x ( std::get<0>( dt ) ), m_y ( std::get<1>( dt ) ), m_z ( 0.0f ) { }

double
Position::GetX ( void ) const
{
    return m_x;
}

double
Position::GetY ( void ) const
{
    return m_y;
}

double
Position::GetZ ( void ) const
{
    return m_z;
}

void
Position::SetX ( double x )
{
    m_x = x;
}

void
Position::SetY ( double y )
{
    m_y = y;
}

void
Position::SetZ ( double z )
{
    m_z = z;
}

Position::operator std::vector<double> ( ) const
{
    return
    {
        m_x, m_y, m_z
    };
}

Position::operator std::vector<float> ( ) const
{
    return
    {
        float(m_x ), float(m_y ), float(m_z )
    };
}

Position::operator std::tuple<double, double, double> ( ) const
{
    return std::make_tuple(m_x, m_y, m_z);
}

Position::operator std::tuple<float, float, float> ( ) const
{
    return std::make_tuple(m_x, m_y, m_z);
}

Position&
        Position::operator= ( std::vector<double> const& dv )
{
    assert(dv.size() >= 2 && "Position (double) vector has not required dimensions");
    m_x = dv[0];
    m_y = dv[1];
    m_z = ( dv.size() > 2 ? dv[2] : 0.0f );
    return *this;
}

Position&
        Position::operator= ( std::vector<float> const& dv )
{
    assert(dv.size() >= 2 && "Position (float) vector has not required dimensions");
    m_x = dv[0];
    m_y = dv[1];
    m_z = ( dv.size() > 2 ? dv[2] : 0.0f );
    return *this;
}

Position&
        Position::operator= ( std::tuple<double, double, double> const& dt )
{
    m_x = std::get<0>( dt );
    m_y = std::get<1>( dt );
    m_z = std::get<2>( dt );
    return *this;
}

Position&
        Position::operator= ( std::tuple<float, float, float> const& dt )
{
    m_x = std::get<0>( dt );
    m_y = std::get<1>( dt );
    m_z = std::get<2>( dt );
    return *this;
}

Position&
        Position::operator= ( std::tuple<double, double> const& dt )
{
    m_x = std::get<0>( dt );
    m_y = std::get<1>( dt );
    return *this;
}

Position&
        Position::operator= ( std::tuple<float, float> const& dt )
{
    m_x = std::get<0>( dt );
    m_y = std::get<1>( dt );
    return *this;
}

Distance
Position::GetPlanarDistance ( Position const& other )
{
    return Distance(units::m(std::sqrt(std::pow(std::fabs(m_x - other.m_x), 2.0) +
            std::pow(std::fabs(m_y - other.m_y), 2.0))));
}

Distance
Position::Get3dDistance ( Position const& other )
{
    return Distance(units::m(std::sqrt(std::pow(std::fabs(m_x - other.m_x), 2.0) +
            std::pow(std::fabs(m_y - other.m_y), 2.0) +
            std::pow(std::fabs(m_z - other.m_z), 2.0))));
}

Distance
Position::GetXDeltaAbs ( Position const& other )
{
    return Distance(units::m(std::fabs(m_x - other.m_x)));
}

Distance
Position::GetYDeltaAbs ( Position const& other )
{
    return Distance(units::m(std::fabs(m_y - other.m_y)));
}

Distance
Position::GetZDeltaAbs ( Position const& other )
{
    return Distance(units::m(std::fabs(m_z - other.m_z)));
}

Distance
Position::GetXDelta ( Position const& other )
{
    return Distance(units::m(m_x - other.m_x ));
}

Distance
Position::GetYDelta ( Position const& other )
{
    return Distance(units::m(m_y - other.m_y ));
}

Distance
Position::GetZDelta ( Position const& other )
{
    return Distance(units::m(m_z - other.m_z ));
}

