#ifndef PRINTUSERS_H
#define PRINTUSERS_H

#include "User.h"

class PrintUsers
{
public:

    enum class PrintType : std::uint8_t
    {
        RSRPMAP = 0,
        RSRPDIST,
        CONSOLE,
        EFF_SINR,
        UL_TXPOWER,
        CONSOLE_UL
    };
    
    enum class UlcpType : std::uint8_t
    {
        OL = 0,
        CL,
        LP,
        NC
    };


    PrintUsers(PrintType type, UlcpType = UlcpType::OL);
    void SetIteration(std::uint32_t iter);
    void operator()(gnsm::Vec_t<User> us);

    const PrintType m_printType;
    const UlcpType m_ulcpType;
private:
    
    std::uint32_t m_currIter;
    std::uint32_t m_nusers;

    struct posRef
    {
        double x;
        double y;
        double val;
    };

    using rsrpMap = std::map<LteCell*, std::vector<posRef>>;

    void PrintRsrpMap(gnsm::Vec_t<User> us);
    void PrintRsrpDist(gnsm::Vec_t<User> us);
    void PrintSinr(gnsm::Vec_t<User> us);
    void PrintTxPower(gnsm::Vec_t<User> us);
    void PrintClosedLoop(gnsm::Vec_t<User> us, std::uint32_t iter);
    void OpenFile(std::ofstream& fs, std::stringstream const& ss);
    void GetName(gnsm::Ptr_t<User> u, std::stringstream& ss, std::string med);
};

#endif /* PRINTUSERS_H */