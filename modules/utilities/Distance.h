#ifndef DISTANCE_H
#define DISTANCE_H

#include "CommonUnits.h"

/**
 * \ingroup core-utils
 * \brief Class to handle distances
 */
class Distance
{
public:
    explicit Distance(units::m d);
    explicit Distance(units::km d);
    
    double GetM ( void ) const;
    double GetKm ( void ) const;
    
    units::m GetUnitsM ( void ) const;
    units::km GetUnitsKm ( void ) const;

private:
    Distance() = delete;
    units::km m_km;

};

#endif /* DISTANCE_H */

