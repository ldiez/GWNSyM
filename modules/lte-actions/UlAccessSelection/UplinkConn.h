#ifndef SIMPLEUPLINK_H
#define SIMPLEUPLINK_H

#include "Power.h"
#include "User.h"

/**
 * \brief Implementation of the uplink association for LTE. It performs an 
 * association with the cell with lowesp PL, provided there are enough RBs
 * 
 */
class UplinkConn
{
public:

    enum class Mode : std::uint8_t
    {
        PL = 0,
        RSRP,
        CRE
    };

    UplinkConn(Mode m = Mode::RSRP);
    void operator()(gnsm::Vec_t<User> users);

private:
    void SelectRsrp(gnsm::Ptr_t<User>& user);
    void SelectPl(gnsm::Ptr_t<User>& user);
    void SelectCre(gnsm::Ptr_t<User>& user);
    Mode m_mode;
};

#endif /* SIMPLEUPLINK_H */