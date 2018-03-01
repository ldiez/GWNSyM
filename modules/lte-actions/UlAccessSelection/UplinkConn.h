#ifndef SIMPLEUPLINK_H
#define SIMPLEUPLINK_H

#include "Power.h"
#include "User.h"

/**
 * \brief Implementation of the uplink association for LTE. It performs an 
 * association with the cell with lowesp PL, provided there are enough RBs
 * 
 */
class UplinkConn {
public:

    enum class Mode : std::uint8_t {
        PL = 0,
        RSRP,
        CRE
    };

    UplinkConn(Mode m = Mode::RSRP, units::dB picoCre = units::dB(0.0),
            units::dB microCre = units::dB(0.0),
            units::dB macroCre = units::dB(0.0));

    void operator()(gnsm::Vec_t<User> users);

private:

    double Nrbs(gnsm::Ptr_t<User>& user);

    void SelectRsrp(gnsm::Ptr_t<User>& user);
    void SelectPl(gnsm::Ptr_t<User>& user);
    void SelectCre(gnsm::Ptr_t<User>& user);

    Mode m_mode;
    units::dB m_picoBias;
    units::dB m_microbias;
    units::dB m_macroBias;
};

#endif /* SIMPLEUPLINK_H */