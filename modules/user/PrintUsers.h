#ifndef PRINTUSERS_H
#define PRINTUSERS_H

#include "User.h"

class PrintUsers {
public:

    enum class PrintType : std::uint8_t {
        RSRPMAP = 0,
        RSRPDIST,
        CONSOLE,
        EFF_SINR,
        UL_TXPOWER,
        CONSOLE_UL,
        CLOSED_LOOP
    };
    
    
    PrintUsers(PrintType type);
    void SetIteration (std::uint32_t iter);
    void operator()(gnsm::Vec_t<User> us);

    const PrintType m_printType;
private:
    std::uint32_t m_currIter;
};

#endif /* PRINTUSERS_H */