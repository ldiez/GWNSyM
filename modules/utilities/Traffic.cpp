#include "Traffic.h"


using namespace units;

Traffic::Traffic() : m_kbps(0.0)
{
}

Traffic::Traffic(bps v) : m_kbps(To<kbps>::Do(v))
{
}

Traffic::Traffic(kbps v) : m_kbps(v)
{
}

Traffic::Traffic(Mbps v) : m_kbps(To<kbps>::Do(v))
{
}

Traffic&
        Traffic::operator=(Traffic const& v)
{
    m_kbps = v.m_kbps;
    return *this;
}

Traffic&
        Traffic::operator=(units::bps const& v)
{
    m_kbps = To<kbps>::Do(v);
    return *this;
}

Traffic&
        Traffic::operator=(units::kbps const& v)
{
    m_kbps = v;
    return *this;
}

Traffic&
        Traffic::operator=(units::Mbps const& v)
{
    m_kbps = To<kbps>::Do(v);
    return *this;
}

Traffic
Traffic::operator+(Traffic const& t) const
{
    return Traffic(m_kbps + t.m_kbps);
}

void
Traffic::operator+=(Traffic const& t)
{
    m_kbps += t.m_kbps;
}

Traffic
Traffic::operator+(bps const& t) const
{
    return Traffic(m_kbps + t);
}

void
Traffic::operator+=(bps const& t)
{
    m_kbps += t;
}

Traffic
Traffic::operator+(kbps const& t) const
{
    return Traffic(m_kbps + t);
}

void
Traffic::operator+=(kbps const& t)
{
    m_kbps += t;
}

Traffic
Traffic::operator+(Mbps const& t) const
{
    return Traffic(m_kbps + t);
}

void
Traffic::operator+=(Mbps const& t)
{
    m_kbps += t;
}

Traffic
Traffic::operator-(Traffic const& t) const
{
    return Traffic(m_kbps - t.m_kbps);
}

void
Traffic::operator-=(Traffic const& t)
{
    m_kbps -= t.m_kbps;
}

Traffic
Traffic::operator-(bps const& t) const
{
    return Traffic(m_kbps - t);
}

void
Traffic::operator-=(bps const& t)
{
    m_kbps -= t;
}

Traffic
Traffic::operator-(kbps const& t) const
{
    return Traffic(m_kbps - t);
}

void
Traffic::operator-=(kbps const& t)
{
    m_kbps -= t;
}

Traffic
Traffic::operator-(Mbps const& t) const
{
    return Traffic(m_kbps - t);
}

void
Traffic::operator-=(Mbps const& t)
{
    m_kbps -= t;
}

bool Traffic::operator<(Traffic const& p) const
{
    return m_kbps < p.m_kbps;
}

bool Traffic::operator<(bps const& p) const
{
    return m_kbps < To<kbps>::Do(p);
}

bool Traffic::operator<(kbps const& p) const
{
    return m_kbps < p;
}

bool Traffic::operator<(Mbps const& p) const
{
    return m_kbps < To<kbps>::Do(p);
}

bool Traffic::operator<=(Traffic const& p) const
{
    return m_kbps <= p.m_kbps;
}

bool Traffic::operator<=(bps const& p) const
{
    return m_kbps <= To<kbps>::Do(p);
}

bool Traffic::operator<=(kbps const& p) const
{
    return m_kbps <= p;
}

bool Traffic::operator<=(Mbps const& p) const
{
    return m_kbps <= To<kbps>::Do(p);
}

bool Traffic::operator>(Traffic const& p) const
{
    return m_kbps > p.m_kbps;
}

bool Traffic::operator>(bps const& p) const
{
    return m_kbps > To<kbps>::Do(p);
}

bool Traffic::operator>(kbps const& p) const
{
    return m_kbps > p;
}

bool Traffic::operator>(Mbps const& p) const
{
    return m_kbps > To<kbps>::Do(p);
}

bool Traffic::operator>=(Traffic const& p) const
{
    return m_kbps >= p.m_kbps;
}

bool Traffic::operator>=(bps const& p) const
{
    return m_kbps >= To<kbps>::Do(p);
}

bool Traffic::operator>=(kbps const& p) const
{
    return m_kbps >= p;
}

bool Traffic::operator>=(Mbps const& p) const
{
    return m_kbps >= To<kbps>::Do(p);
}

bool Traffic::operator==(Traffic const& p) const
{
    return m_kbps == p.m_kbps;
}

bool Traffic::operator==(bps const& p) const
{
    return m_kbps == To<kbps>::Do(p);
}

bool Traffic::operator==(kbps const& p) const
{
    return m_kbps == p;
}

bool Traffic::operator==(Mbps const& p) const
{
    return m_kbps == To<kbps>::Do(p);
}

bool Traffic::operator!=(Traffic const& p) const
{
    return m_kbps != p.m_kbps;
}

bool Traffic::operator!=(bps const& p) const
{
    return m_kbps != To<kbps>::Do(p);
}

bool Traffic::operator!=(kbps const& p) const
{
    return m_kbps != p;
}

bool Traffic::operator!=(Mbps const& p) const
{
    return m_kbps != To<kbps>::Do(p);
}

double
Traffic::GetBps(void) const
{
    return To<bps>::Do(m_kbps).RawVal();
}

double
Traffic::GetKbps(void) const
{
    return m_kbps.RawVal();
}

double
Traffic::GetMbps(void) const
{
    return To<Mbps>::Do(m_kbps).RawVal();
}

bps
Traffic::ToBps()
{
    return To<bps>::Do(m_kbps);
}

kbps
Traffic::ToKbps()
{
    return m_kbps;
}

Mbps
Traffic::ToMbps()
{
    return To<Mbps>::Do(m_kbps);
}