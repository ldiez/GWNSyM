#ifndef POWERPARSER_H
#define POWERPARSER_H


#include <cassert>
#include <string>
#include <unordered_map>
#include "Log.h"
#include "Power.h"

/**
 * \ingroup
 * \brief Utilities to parser a power magnitude to given units
 */

namespace 
{
enum class AuxPowerPref_s 
{
    W, mW, dBW, dBm
};

static const std::unordered_map<std::string, AuxPowerPref_s>
        PowerUnitsParser_s = {
    {"W", AuxPowerPref_s::W},
    {"Watt", AuxPowerPref_s::W},
    {"mW", AuxPowerPref_s::mW},
    {"MilliWatt", AuxPowerPref_s::mW},
    {"dBW", AuxPowerPref_s::dBW},
    {"dBm", AuxPowerPref_s::dBm},
};
} 

inline Power
ParsePow(std::string const& units, double value)
{
    try
    {
        switch (PowerUnitsParser_s.at(units))
        {
        case AuxPowerPref_s ::W:
            return Power(units::Watt(value));
        case AuxPowerPref_s::mW:
            return Power(units::MilliWatt(value));
        case AuxPowerPref_s::dBW:
            return Power(units::dBW(value));
        case AuxPowerPref_s::dBm:
            return Power(units::dBm(value));
        default:
            assert(false && "Power units UNKNOWN");
        }
    }
    catch (std::exception& exc)
    {
        MSG_ASSERT(false, "Power unit " + units + " is not known");
        assert(false && "Power units UNKNOWN");
    }
}

#endif /* POWERPARSER_H */

