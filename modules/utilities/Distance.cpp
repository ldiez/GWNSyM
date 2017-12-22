#include "Distance.h"

Distance::Distance(units::m d) : m_km(units::To<units::km>::Do(d))
{
}

Distance::Distance(units::km d) : m_km(d)
{
}

double
Distance::GetM(void) const
{
    return units::To<units::m>::Do(m_km).RawVal();
}

double
Distance::GetKm(void) const
{
    return m_km.RawVal();
}

units::m
Distance::GetUnitsM(void) const
{
    return units::To<units::m>::Do(m_km);
}

units::km
Distance::GetUnitsKm(void) const
{
    return m_km;
}