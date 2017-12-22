#ifndef TRAFFICPARSER_H
#define TRAFFICPARSER_H

#include <cassert>
#include <string>
#include <unordered_map>
#include "Log.h"
#include "Traffic.h"

/**
 * \ingroup
 * \brief Utilities to parse a traffic magnitude to given units
 */

namespace 
{
enum class AuxTrafficPref_s 
{
    bps, kbps, Mbps
};

static const std::unordered_map<std::string, AuxTrafficPref_s>
        PowerUnitsParser_s = {
    {"bps", AuxTrafficPref_s::bps},
    {"kbps", AuxTrafficPref_s::kbps},
    {"Mbps", AuxTrafficPref_s::Mbps},
};
} 

inline Traffic
ParseTraffic(std::string const& units, double value)
{
    try
    {
        switch (PowerUnitsParser_s.at(units))
        {
        case AuxTrafficPref_s ::bps:
            return Traffic(units::bps(value));
        case AuxTrafficPref_s::kbps:
            return Traffic(units::kbps(value));
        case AuxTrafficPref_s::Mbps:
            return Traffic(units::Mbps(value));
        default:
            assert(false && "Traffic units UNKNOWN (bps, kbps, Mbps)");
        }
    }
    catch (std::exception& exc)
    {
        MSG_ASSERT(false, "Traffic unit " + units + " is not known");
        assert(false && "Traffic units UNKNOWN");
    }
}


#endif /* TRAFFICPARSER_H */

