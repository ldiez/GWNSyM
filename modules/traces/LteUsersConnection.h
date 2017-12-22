#ifndef LTEUSERSCONNECTION_H
#define LTEUSERSCONNECTION_H

#include "User.h"
#include "Trace.h"

/**
 * \description Create a trace with information of the users connection. 
 * The trace is step-wise.
 * 
 * For each snapshot it creates a trace file with:
 * + Number of cells the user connects to
 * + Traffic demand
 * + Thput achieved in by the current connection
 * + Number of RBs
 * 
 */
class LteUsersConnection
{
public:
    /**
     * \brief Ctor
     * \param path --> Path of the trace files
     * \param tail --> Trace tail 
     */
    LteUsersConnection(std::string const& path, std::string const& tail = "");
    void SetIteration ( std::uint32_t iter);
    
    void operator()(gnsm::Vec_t<User>);
private:
    std::string MakeStr (gnsm::Ptr_t<User> const&);
    
    Trace m_trace;
    std::uint32_t m_iter;

};

#endif /* USERSCONNECTION_H */

