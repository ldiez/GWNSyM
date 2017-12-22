#ifndef SIMPLEUPLINK_H
#define SIMPLEUPLINK_H

#include "Power.h"
#include "User.h"

/**
 * \brief Implementation of the uplink association for LTE.
 * 
 * Power control:
 * P_{tx} = min {P_{max}, P_0 + 10*log(M) + \alpha * PL + \delta_{mcs} + f(\Delta_i)}
 * 
 * The number of RBs assigned for a user in the uplink are defined upon a target SINR.
 * 
 * Cap = Nrb * log2(1+ SINR)
 * 
 * SINR = P_0 * PL^(1-\alpha)/Noise
 * 
 * It sets the number of 
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

