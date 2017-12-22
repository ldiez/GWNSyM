#ifndef PARALLELLTESCAN_H
#define PARALLELLTESCAN_H

#include "CoreUtils.h"
#include "lte-ae/LteEnb.h"
#include "User.h"
#include "RxPower.h"

/**
 * \description This class performs the scanning procedure for LTE adding multi-thread 
 * functionality to boost it.
 */

class ParallelLteScan
{
public:
    /**
     * \brief Ctor
     * \param ant --> Set whether the antenna uses horizontal pattern of horizontal and vertical
     * \param prop --> Set type of propagation: only LOS, only NLOS or both (statistical)
     */
    ParallelLteScan(AntennaType_e ant = AntennaType_e::HONLY,
                    PropType_e prop = PropType_e::FULL);
    void operator()(gnsm::Vec_t<User> users, gnsm::Vec_t<LteEnb> enbs);

private:
    /**
     * \brief Perform all the scanning procedure for one user
     * \param user --> User
     * \param enbs --> LTE eNBs container
     */
    void DoUserScan(gnsm::Ptr_t<User> users, gnsm::Vec_t<LteEnb> enbs);
    
    AntennaType_e m_antennaType;
    PropType_e m_propType;
};

#endif /* PARALLELLTESCAN */

