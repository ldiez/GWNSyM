#ifndef LTECOMMON_H
#define LTECOMMON_H

#include <ostream>

enum class EnbType : std::uint8_t
{
    MACRO = 0,
    MICRO,
    PICO,
    NONE
};



inline std::ostream& operator<<(std::ostream& os, EnbType const& et)
{
    os << (et == EnbType::MACRO ? "MACRO" : et == EnbType::MICRO ? "MICRO" : "PICO");
    return os;
}

inline std::string ToStr(EnbType const& et)
{
    return (et == EnbType::MACRO ? "MACRO" : et == EnbType::MICRO ? "MICRO" : "PICO");
}


#endif /* LTECOMMON_H */

