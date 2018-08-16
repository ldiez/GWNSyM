#ifndef LTEUECONF_H
#define	LTEUECONF_H

#include <string>
#include "Power.h"

class LteUeConf
{
public:
    LteUeConf ( std::string&& filename, double alpha = 0.0 );
    
    /**
     * \brief Noise figure of the user terminal in dB
     * \return <-- NF
     */
    units::dB GetNoiseFigure ( void ) const;
    
    /**
     * \brief Reception gain of the user terminal in dB
     * \return <-- Rx gain
     */
    units::dB GetRxGain ( void ) const;
    
    /**
     * \brief Transmission gain of the user terminal in dB
     * \return <-- Tx gain
     */
    units::dB GetTxGain ( void ) const;
    
    /**
     * \brief Give the SINR threshold for the UL
     * \return <-- SINR in dB
     */
    units::dB UlSinrTh ( void ) const;
    
    /**
     * \brief Get maximum transmission power
     */
    Power GetPmax ( void ) const;
    
    /**
     * \brief Get compensation pathloss factor
     */
    double GetAlpha ( void ) const;
    
private:
    units::dB m_rxGain;
    units::dB m_txGain;
    units::dB m_noiseFigure;
    units::dB m_ulSinrTh;
    Power m_pMax;
    double m_alpha;

};
#endif	/* LTEUECONF_H */