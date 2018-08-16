#ifndef ULCLOSESTCONN_H
#define ULCLOSESTCONN_H

#include "CoreUtils.h"


class User;
class LteEnb;
class UlClosestConn
{
public:
    UlClosestConn();
    
    void operator()(gnsm::Vec_t<User> users, gnsm::Vec_t<LteEnb> enbs);
private:

};

#endif /* ULCLOSESTCONN_H */

