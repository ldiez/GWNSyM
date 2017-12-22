#ifndef RXPOWER_H
#define RXPOWER_H

#include "User.h"
#include "lte-ae/LteEnb.h"


enum class AntennaType_e : std::uint8_t
{
    HONLY,
    HV
};

enum class PropType_e : std::uint8_t
{
    LOS,
    NLOS,
    FULL
};

/**
 * \brief Calculate received power at each user from each base station (sectors).
 * This class compute the received power by calculating:
 * P_rx = P_tx -max [PathLoss + cl - Gtx - Grx, MCL]
 * 
 * The pathloss is calculated assuming an urban scenario, and the type of eNB 
 * is taken to use the proper model (Macro, Micro, Pico)
 */
class RxPower
{
public:

    /**
     * \brief Ctor
     * \param ant --> Set whether the antenna uses horizontal pattern of horizontal and vertical
     * \param prop --> Set type of propagation: only LOS, only NLOS or both (statistical)
     */
    RxPower(AntennaType_e ant, PropType_e prop);

    void operator()(gnsm::Ptr_t<User> user, gnsm::Ptr_t<LteEnb> ae);
private:
    const AntennaType_e m_antennaType;
    const PropType_e m_propType;
    
};

void LteRxPower(gnsm::Ptr_t<User> user, gnsm::Ptr_t<LteEnb> ae,
                AntennaType_e ant, PropType_e prop);

#endif /* RXPOWER_H */