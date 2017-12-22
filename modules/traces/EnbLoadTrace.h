#ifndef ENBLOADTRACE_H
#define ENBLOADTRACE_H

#include "Trace.h"
#include "lte-ae/LteEnb.h"

/**
 * \brief Make the traces of the load of the eNBs.
 * The trace files are stored in a defined path:
 * For \each \snapshot it creates a file with each 
 * type of eNB cell (MACRO, MICRO, PICO):
 * e.g. enb-load_PICO_...
 * 
 * \Note Existing files with the same name are overwritten
 */
class EnbLoadTrace
{
public:
    /**
     * \brief Ctor
     * \param path --> Path to store the files
     * \param tail --> Tail for the name as defined in the \Trace class
     */
    EnbLoadTrace(std::string const& path, std::string const& tail = "");
    
    void SetIteration (std::uint32_t iter);
    void operator() (gnsm::Vec_t<LteEnb> enbs);
    
private:
    std::string MakeStr (gnsm::Ptr_t<LteEnb> const& enb);
    
    std::uint32_t m_iter;
    Trace m_macroTr;
    Trace m_microTr;
    Trace m_picoTr;

};

#endif /* ENBLOADTRACE_H */