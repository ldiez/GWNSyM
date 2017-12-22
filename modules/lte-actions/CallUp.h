#ifndef CALLUP_H
#define CALLUP_H

#include "User.h"
#include "lte-ae/LteEnb.h"


/**
 * \description These classes are in charge of prepare the entities to perform a new iteration.
 */


struct UserCallUp
{
    void SetIteration(std::uint32_t){}
    void operator () (gnsm::Ptr_t<User> user)
    {
        user->CallUp();
    }
};

struct EnbCallUp
{
    void SetIteration(std::uint32_t){}
    void operator () (gnsm::Ptr_t<LteEnb> enb)
    {
        enb->CallUp();
    }
};

#endif /* CALLUP_H */