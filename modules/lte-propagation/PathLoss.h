#ifndef LTELINKQUALITY_H
#define	LTELINKQUALITY_H
#include <random>
#include "LteUtils.h"
#include "Distance.h"

namespace LTE
{

/**
 * \ingroup LTE
 * \brief Calculation of pathloss between the user equipments and the access element for 
 * LTE scenarios
 */

/**
 * \brief Path loss for urban macro cell
 * \param distance --> Distance between EU and BS
 * \param frec --> Working frequency
 * \param txPpower --> Transmission power in dB
 * \param hbs --> Height of the base station
 * \param hut --> Height of the user equipment
 * \return <-- Received signal power in dB
 */
units::dB UmaPathloss ( units::m distance, units::GHz frec, units::m hbs, units::m hut );
units::dB UmaPathloss_LOS ( units::m distance, units::GHz frec, units::m hbs, units::m hut );
units::dB UmaPathloss_NLOS ( units::m distance, units::GHz frec, units::m hbs, units::m hut );

/**
 * \brief Path loss for urban micro cell 
 * \param distance --> Distance between EU and BS
 * \param frec --> Working frequency
 * \param txPpower --> Transmission power in dB
 * \param hbs --> Height of the base station
 * \param hut --> Height of the user equipment
 * \return <-- Received signal power in dB
 */
units::dB UmiPathloss ( units::m distance, units::GHz frec, units::m hbs, units::m hut );
units::dB UmiPathloss_LOS ( units::m distance, units::GHz frec, units::m hbs, units::m hut );
units::dB UmiPathloss_NLOS ( units::m distance, units::GHz frec, units::m hbs, units::m hut );


/**
 * \brief Path loss for urban pico cells
 * \param distance --> Distance between the eNB and the UE
 * \param frec --> Working frequency
 * \param hbs --> Height of the eNB
 * \param hut --> Height of the UE
 * \return <-- Received signal power in db
 */
units::dB UpiPathloss ( units::m distance, units::GHz frec, units::m hbs, units::m hut);
units::dB UpiPathloss_LOS ( units::m distance, units::GHz frec, units::m hbs, units::m hut);
units::dB UpiPathloss_NLOS ( units::m distance, units::GHz frec, units::m hbs, units::m hut);
} // namespace LTE
#endif	/* LTELINKQUALITY_H */