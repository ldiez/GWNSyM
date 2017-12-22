#include "Bandwidth.h"

using namespace units;

Bandwidth::Bandwidth(MHz v) : m_khz(To<kHz>::Do(v))
{
}

Bandwidth::Bandwidth(kHz v) : m_khz(v)
{
}

Bandwidth::Bandwidth(Hz v) : m_khz(To<kHz>::Do(v))
{
}

Bandwidth::Bandwidth(dBMHz v) : m_khz(To<kHz>::Do(v))
{
}

Bandwidth::Bandwidth(dBkHz v) : m_khz(To<kHz>::Do(v))
{
}

Bandwidth::Bandwidth(dBHz v) : m_khz(To<kHz>::Do(v))
{
}

bool
Bandwidth::operator<(Bandwidth const& p) const
{
    return m_khz < p.m_khz;
}

bool
Bandwidth::operator<(Hz const& p) const
{
    return m_khz < To<kHz>::Do(p);
}

bool
Bandwidth::operator<(kHz const& p) const
{
    return m_khz < p;
}

bool
Bandwidth::operator<(MHz const& p) const
{
    return m_khz < To<kHz>::Do(p);
}

bool
Bandwidth::operator<(dBHz const& p) const
{
    return m_khz < To<kHz>::Do(p);
}

bool
Bandwidth::operator<(dBkHz const& p) const
{
    return m_khz < To<kHz>::Do(p);
}

bool
Bandwidth::operator<(dBMHz const& p) const
{
    return m_khz < To<kHz>::Do(p);
}

bool
Bandwidth::operator<=(Bandwidth const& p) const
{
    return m_khz <= p.m_khz;
}

bool Bandwidth::operator<=(Hz const& p) const
{
    return m_khz <= To<kHz>::Do(p);
}

bool Bandwidth::operator<=(kHz const& p) const
{
    return m_khz <= p;
}

bool Bandwidth::operator<=(MHz const& p) const
{
    return m_khz <= To<kHz>::Do(p);
}

bool Bandwidth::operator<=(dBHz const& p) const
{
    return m_khz <= To<kHz>::Do(p);
}

bool Bandwidth::operator<=(dBkHz const& p) const
{
    return m_khz <= To<kHz>::Do(p);
}

bool Bandwidth::operator<=(dBMHz const& p) const
{
    return m_khz <= To<kHz>::Do(p);
}

bool Bandwidth::operator>(Bandwidth const& p) const
{
    return m_khz > p.m_khz;
}

bool Bandwidth::operator>(Hz const& p) const
{
    return m_khz > To<kHz>::Do(p);
}

bool Bandwidth::operator>(kHz const& p) const
{
    return m_khz > p;
}

bool Bandwidth::operator>(MHz const& p) const
{
    return m_khz > To<kHz>::Do(p);
}

bool Bandwidth::operator>(dBHz const& p) const
{
    return m_khz > To<kHz>::Do(p);
}

bool Bandwidth::operator>(dBkHz const& p) const
{
    return m_khz > To<kHz>::Do(p);
}

bool Bandwidth::operator>(dBMHz const& p) const
{
    return m_khz > To<kHz>::Do(p);
}

bool Bandwidth::operator>=(Bandwidth const& p) const
{
    return m_khz >= p.m_khz;
}

bool Bandwidth::operator>=(Hz const& p) const
{
    return m_khz >= To<kHz>::Do(p);
}

bool Bandwidth::operator>=(kHz const& p) const
{
    return m_khz >= p;
}

bool Bandwidth::operator>=(MHz const& p) const
{
    return m_khz >= To<kHz>::Do(p);
}

bool Bandwidth::operator>=(dBHz const& p) const
{
    return m_khz >= To<kHz>::Do(p);
}

bool Bandwidth::operator>=(dBkHz const& p) const
{
    return m_khz >= To<kHz>::Do(p);
}

bool Bandwidth::operator>=(dBMHz const& p) const
{
    return m_khz >= To<kHz>::Do(p);
}

bool Bandwidth::operator==(Bandwidth const& p) const
{
    return m_khz == p.m_khz;
}

bool Bandwidth::operator==(Hz const& p) const
{
    return m_khz == To<kHz>::Do(p);
}

bool Bandwidth::operator==(kHz const& p) const
{
    return m_khz == p;
}

bool Bandwidth::operator==(MHz const& p) const
{
    return m_khz == To<kHz>::Do(p);
}

bool Bandwidth::operator==(dBHz const& p) const
{
    return m_khz == To<kHz>::Do(p);
}

bool Bandwidth::operator==(dBkHz const& p) const
{
    return m_khz == To<kHz>::Do(p);
}

bool Bandwidth::operator==(dBMHz const& p) const
{
    return m_khz == To<kHz>::Do(p);
}

bool Bandwidth::operator!=(Bandwidth const& p) const
{
    return m_khz != p.m_khz;
}

bool Bandwidth::operator!=(Hz const& p) const
{
    return m_khz != To<kHz>::Do(p);
}

bool Bandwidth::operator!=(kHz const& p) const
{
    return m_khz != p;
}

bool Bandwidth::operator!=(MHz const& p) const
{
    return m_khz != To<kHz>::Do(p);
}

bool Bandwidth::operator!=(dBHz const& p) const
{
    return m_khz != To<kHz>::Do(p);
}

bool Bandwidth::operator!=(dBkHz const& p) const
{
    return m_khz != To<kHz>::Do(p);
}

bool Bandwidth::operator!=(dBMHz const& p) const
{
    return m_khz != To<kHz>::Do(p);
}

double
Bandwidth::GetMhz(void) const
{
    return To<MHz>::Do(m_khz).RawVal();
}

double
Bandwidth::GetKhz(void) const
{
    return m_khz.RawVal();
}

double
Bandwidth::GetHz(void) const
{
    return To<Hz>::Do(m_khz).RawVal();
}

double
Bandwidth::GetDbMhz(void) const
{
    return To<dBMHz>::Do(m_khz).RawVal();
}

double
Bandwidth::GetDbKhz(void) const
{
    return To<dBkHz>::Do(m_khz).RawVal();
}

double
Bandwidth::GetDbHz(void) const
{
    return To<dBHz>::Do(m_khz).RawVal();
}