
#ifndef SERVCOMMON_H
#define SERVCOMMON_H

#include <ostream>
#include <unordered_map>

enum class ServType : std::uint8_t
{
    GENERIC = 0,
    VIDEO,
    VOICE,
    INTERNET
};

inline std::ostream& operator<<(std::ostream& os, ServType const& et)
{
    os << (et == ServType::GENERIC ? "GENERIC" :
            et == ServType::INTERNET ? "INTERNET" :
            et == ServType::VIDEO ? "VIDEO" : "VOICE"
            );
    return os;
}

inline std::string ToStr(ServType const& et)
{
    return (et == ServType::GENERIC ? "GENERIC" :
            et == ServType::INTERNET ? "INTERNET" :
            et == ServType::VIDEO ? "VIDEO" : "VOICE"
            );
}


#endif /* SERVCOMMON_H */