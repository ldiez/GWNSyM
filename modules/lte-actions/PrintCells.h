#ifndef PRINTCELLS_H
#define PRINTCELLS_H

#include "lte-ae/LteEnb.h"

class PrintCells
{
public:

    enum class PrintType : std::uint8_t
    {
        POSITION = 0,
    };

    PrintCells(PrintType type, std::string const& head = "",
               std::string const& tail = "");
    void operator()(gnsm::Vec_t<LteEnb> layer);


private:
    const PrintType m_type;
    const std::string m_head;
    const std::string m_tail;

};


#endif /* PRINTCELLS_H */

