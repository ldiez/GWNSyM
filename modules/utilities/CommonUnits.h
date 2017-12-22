#ifndef COMMONUNITS_H
#define COMMONUNITS_H

#include "Units.h"


/**
 * \ingroup units
 * \brief Definition of different magnitudes to be used
 */
namespace units
{

// Frequency and bandwidth units
using GHz = LinVal<LinPref::Giga>;
using MHz = LinVal<LinPref::Mega>;
using kHz = LinVal<LinPref::Kilo>;
using Hz = LinVal<LinPref::Ref>;
using dBMHz = LogVal<LogPref::dBM>;
using dBkHz = LogVal<LogPref::dBk>;
using dBHz = LogVal<LogPref::dB>;

// power units
using Watt = LinVal<LinPref::Ref>;
using MilliWatt = LinVal<LinPref::Milli>;
using dBW = LogVal<LogPref::dB>;
using dBm = LogVal<LogPref::dBm>;
using dB = LogVal<LogPref::dB>;

// traffic units
using bps = LinVal<LinPref::Ref>;
using kbps = LinVal<LinPref::Kilo>;
using Mbps = LinVal<LinPref::Mega>;

// distance units
using km = LinVal<LinPref::Kilo>;
using m = LinVal<LinPref::Ref>;
}

#endif /* COMMONUNITS_H */