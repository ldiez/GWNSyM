#ifndef SIMPLEUPLINK_H
#define SIMPLEUPLINK_H

#include "Power.h"
#include "User.h"

/**
 * \brief Implementation of the uplink association for LTE. It performs an 
 * association with the cell with lowesp PL, provided there are enough RBs
 * 
 */
class SimpleUplink
{
public:
    SimpleUplink();
    
    void operator () ( gnsm::Vec_t<User> users );
    
private:
    void Select (gnsm::Ptr_t<User>& user);
    
    double Nrbs (double sinr, gnsm::Ptr_t<User>& user);
    

};

#endif /* SIMPLEUPLINK_H */

