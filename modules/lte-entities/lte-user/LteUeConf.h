#ifndef LTEUECONF_H
#define	LTEUECONF_H

#include <string>
#include "Power.h"

class LteUeConf
{
public:
    LteUeConf ( std::string&& filename );
    
    /**
     * \brief Noise figure of the user terminal in dB
     * \return <-- NF
     */
    units::dB ReadNoiseFigure ( void ) const;
    
    /**
     * \brief Reception gain of the user terminal in dB
     * \return <-- Rx gain
     */
    units::dB ReadRxGain ( void ) const;
    
    /**
     * \brief Give the SINR threshold for the UL
     * \return <-- SINR in dB
     */
    units::dB UlSinrTh ( void ) const;
    
private:
    units::dB m_rxGain;
    units::dB m_noiseFigure;
    units::dB m_ulSinrTh;

};
#endif	/* LTEUECONF_H */