#ifndef LTECELLCONF_H
#define	LTECELLCONF_H

#include <string>
#include "../LteCommon.h"
#include "Power.h"

class LteCellConf
{
public:
    LteCellConf ( EnbType enbType, std::string const& path);
    
    // provide access to configured parameters
    Power GetTxPower ( void ) const;
    Power GetTxpowerPerRb ( void ) const;
    Power GetTxpowerPerRe ( void ) const;
    double GetCapacity ( void ) const; // capacity in number of RBs
    
    units::dB GetTxGain ( void ) const;
    units::dB GetRxGain ( void ) const;
    
    EnbType GetType (void ) const;
    std::string GetTypeName (void ) const;;
    
    units::dB GetNoiseFigure ( void ) const;
private:
    EnbType m_type;
    Power m_txPower; 
    Power m_txPowerPerRb; 
    Power m_txPowerPerRe; 
    double m_capacity; // number of resource block
    units::dB m_txGain; 
    units::dB m_rxGain; 
    units::dB m_nf; // noise figure
};

#endif	/* LTECELLCONF_H */

