#ifndef SIMPLEUPLINK_H
#define SIMPLEUPLINK_H

#include "Power.h"
#include "User.h"

/**
 * \brief Implementation of the uplink association for LTE. It performs an 
 * association with the cell with lowesp PL, provided there are enough RBs
 * 
 */
class SimpleUplink {
public:

    enum class Mode : std::uint8_t {
        PL = 0,
        RSRP,
        CRE
    };

    SimpleUplink(Mode m = Mode::RSRP, units::dB bias = units::dB(0));

    void operator()(gnsm::Vec_t<User> users);

private:

    double Nrbs(double sinr, gnsm::Ptr_t<User>& user);
    [[deprecated("Do not use it anymore")]]
    void Select(gnsm::Ptr_t<User>& user);

    void SelectRsrp(gnsm::Ptr_t<User>& user);
    void SelectPl(gnsm::Ptr_t<User>& user);
    void SelectCre(gnsm::Ptr_t<User>& user);

    Mode m_mode;
    units::dB m_bias;
};

#endif /* SIMPLEUPLINK_H */