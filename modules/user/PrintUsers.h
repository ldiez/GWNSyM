#ifndef PRINTUSERS_H
#define PRINTUSERS_H

#include "User.h"



class PrintUsers {
public:
    
    PrintUsers();
    
    void operator() (gnsm::Vec_t<User> us);
private:
    

};

#endif /* PRINTUSERS_H */

