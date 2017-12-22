#include "Power.h"

using namespace units;

Power::Power(Watt w) : m_w(w)
{
}

Power::Power(MilliWatt mw) : m_w(To<Watt>::Do(mw))
{
}

Power::Power(dBW d) : m_w(To<Watt>::Do(d))
{
}

Power::Power(dBm d) : m_w(To<Watt>::Do(d))
{
}

Power::Power(Power const& other) : m_w(other.m_w)
{
}

Power& 
Power::operator=(Power const& p)
{
    m_w = p.m_w;
    return *this;
}

Power& 
Power::operator=(Watt const& p)
{
    m_w = p;
    return *this;   
}

Power& 
Power::operator=(MilliWatt const& p)
{
    m_w = To<Watt>::Do(p);
    return *this;
}

Power& 
Power::operator=(dBW const& p)
{
    m_w = To<Watt>::Do(p);
    return *this;
}

Power& 
Power::operator=(units::dBm const& p)
{
    m_w = To<Watt>::Do(p);
    return *this;
}

Power
Power::operator+(Power const& p) const
{
    return Power(m_w + p.m_w);
}

void
Power::operator+=(Power const& p)
{
    m_w += p.m_w;
}

Power
Power::operator-(Power const& p) const
{
    return Power(m_w - p.m_w);
}

void
Power::operator-=(Power const& p)
{
    m_w -= p.m_w;
}

Power
Power::operator+(Watt const& w) const
{
    return Power(m_w + w);
}

void
Power::operator+=(Watt const& w)
{
    m_w += w;
}

Power
Power::operator+(MilliWatt const& mw) const
{
    return Power(m_w + mw);
}

void
Power::operator+=(MilliWatt const& mw)
{
    m_w += mw;
}

Power
Power::operator+(dBW const& dbw) const
{
    return Power(m_w + To<Watt>::Do(dbw));
}

void
Power::operator+=(dBW const& dbw)
{
    m_w += To<Watt>::Do(dbw);
}

Power
Power::operator+(dBm const& dbm) const
{
    return Power(m_w + To<Watt>::Do(dbm));
}

void
Power::operator+=(dBm const& dbm)
{
    m_w += To<Watt>::Do(dbm);
}

Power
Power::operator-(Watt const& w) const
{
    return Power(m_w - w);
}

void
Power::operator-=(Watt const& w)
{
    m_w -= w;
}

Power
Power::operator-(MilliWatt const& mw) const
{
    return Power(m_w - mw);
}

void
Power::operator-=(MilliWatt const& mw)
{
    m_w -= mw;
}

Power
Power::operator-(dBW const& dbw) const
{
    return Power(m_w - To<Watt>::Do(dbw));
}

void
Power::operator-=(dBW const& dbw)
{
    m_w -= To<Watt>::Do(dbw);
}

Power
Power::operator-(dBm const& dbm) const
{
    return Power(m_w - To<Watt>::Do(dbm));
}

void
Power::operator-=(dBm const& dbm)
{
    m_w -= To<Watt>::Do(dbm);
}

bool Power::operator<(Power const& p) const
{
    return m_w > p.m_w;
}

bool Power::operator<(Watt const& p) const
{
    return m_w < p;
}

bool Power::operator<(MilliWatt const& p) const
{
    return m_w < To<Watt>::Do(p);
}

bool Power::operator<(dBW const& p) const
{
    return m_w < To<Watt>::Do(p);
}

bool Power::operator<(dBm const& p) const
{
    return m_w < To<Watt>::Do(p);
}

bool Power::operator<=(Power const& p) const
{
    return m_w <= p.m_w;
}

bool Power::operator<=(Watt const& p) const
{
    return m_w <= p;
}

bool Power::operator<=(MilliWatt const& p) const
{
    return m_w <= To<Watt>::Do(p);
}

bool Power::operator<=(dBW const& p) const
{
    return m_w <= To<Watt>::Do(p);
}

bool Power::operator<=(dBm const& p) const
{
    return m_w <= To<Watt>::Do(p);
}

bool Power::operator>(Power const& p) const
{
    return m_w > p.m_w;
}

bool Power::operator>(Watt const& p) const
{
    return m_w > p;
}

bool Power::operator>(MilliWatt const& p) const
{
    return m_w > To<Watt>::Do(p);
}

bool Power::operator>(dBW const& p) const
{
    return m_w > To<Watt>::Do(p);
}

bool Power::operator>(dBm const& p) const
{
    return m_w > To<Watt>::Do(p);
}

bool Power::operator>=(Power const& p) const
{
    return m_w >= p.m_w;
}

bool Power::operator>=(Watt const& p) const
{
    return m_w >= p;
}

bool Power::operator>=(MilliWatt const& p) const
{
    return m_w > To<Watt>::Do(p);
}

bool Power::operator>=(dBW const& p) const
{
    return m_w > To<Watt>::Do(p);
}

bool Power::operator>=(dBm const& p) const
{
    return m_w > To<Watt>::Do(p);
}

bool Power::operator==(Power const& p) const
{
    return m_w == p.m_w;
}

bool Power::operator==(Watt const& p) const
{
    return m_w == p;
}

bool Power::operator==(MilliWatt const& p) const
{
    return m_w == To<Watt>::Do(p);
}

bool Power::operator==(dBW const& p) const
{
    return m_w == To<Watt>::Do(p);
}

bool Power::operator==(dBm const& p) const
{
    return m_w == To<Watt>::Do(p);
}

bool Power::operator!=(Power const& p) const
{
    return m_w != p.m_w;
}

bool Power::operator!=(Watt const& p) const
{
    return m_w != p;
}

bool Power::operator!=(MilliWatt const& p) const
{
    return m_w != To<Watt>::Do(p);
}

bool Power::operator!=(dBW const& p) const
{
    return m_w != To<Watt>::Do(p);
}

bool Power::operator!=(dBm const& p) const
{
    return m_w != To<Watt>::Do(p);
}

void
Power::Att(double a)
{
    m_w *= 1.0 / a;
}

void
Power::Att(dB const& d)
{
    m_w *= 1.0 / Log2lin(d.RefVal());
}

void
Power::Amp(double a)
{
    m_w *= a;
}

void
Power::Amp(dB const& d)
{
    m_w *= Log2lin(d.RefVal());
}

double
Power::GetWatt(void) const
{
    return m_w.RawVal();
}

double
Power::GetMilliWatt(void) const
{
    return To<MilliWatt>::Do(m_w).RawVal();
}

double
Power::GetDbw(void) const
{
    return To<dBW>::Do(m_w).RawVal();
}

double
Power::GetDbm(void) const
{
    return To<dBm>::Do(m_w).RawVal();
}