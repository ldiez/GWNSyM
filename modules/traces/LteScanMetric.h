#ifndef LTESCANMETRIC_H
#define LTESCANMETRIC_H

#include "User.h"
#include "lte-ae/LteEnb.h"
#include "Trace.h"

/**
 * \description It creates a trace with the result of scanning 
 * the current scenario:
 * - A file with the location of the cells: "cells-location"
 * - A file per snapshot with RSRP measured by each user from all the cells: "users-scan"
 */
class LteScanMetric
{
public:
    /**
     * \brief Ctor
     * \param path --> Path for the directory to store the results. 
     * \param usersFileName --> Name of the users results file. The filename will have 
     * the name passed around followed by "_xxxx" where "xxxx" is the iteration number.
     * The resulting file location is:
     * path/usersFileName_xxx.tr
     * \param aesFileName --> Name of the access elements filename. If the name is not specified, 
     * the file is not created.
     * The resulting file location is:
     * path/aesFileName.tr
     * \param ext --> Extension of the results
     */
    LteScanMetric(std::string const& path, std::string const& tail = "");
    void SetIteration ( std::uint32_t iter);
    
    void operator()(gnsm::Vec_t<User> users, gnsm::Vec_t<LteEnb> enbs);


private:
    std::uint32_t m_iter;
    Trace m_usersTrace;
    Trace m_enbsTrace;
    
    void PrintCells(gnsm::Vec_t<LteEnb> const& enbs);
    void PrintUsers(gnsm::Vec_t<User> const& users);
    std::string MakeUserStr(gnsm::Ptr_t<User> const& user);
};

#endif /* LTESCANMETRIC_H */