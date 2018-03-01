#ifndef PRINTUSERS_H
#define PRINTUSERS_H

#include "User.h"

class PrintUsers {
public:

    enum class PrintType : std::uint8_t {
        RSRPMAP = 0,
        RSRPDIST,
        CONSOLE,
        EFF_SINR
    };
    PrintUsers(PrintType type);
    void operator()(gnsm::Vec_t<User> us);

    const PrintType m_printType;
};

#endif /* PRINTUSERS_H */