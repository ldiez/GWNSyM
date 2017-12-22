#ifndef LTECLUSTER_H
#define LTECLUSTER_H

#include "../lte-ae/LteEnb.h"
#include "LteClusterConf.h"

/**
 * \brief Implementation of a cluster containing one macro and a configurable 
 * number of picos
 */
class LteCluster
{
public:
    LteCluster(gnsm::Id_t id );
    void SetConfiguration ( LteClusterConf const& conf);
    void Aggregate ( std::string const& name, gnsm::ts::Wrapper_t o );
    
    /**
     * \brief Provide access to the macro eNB
     * \return <-- Pointer to the macro cell
     */
    gnsm::Ptr_t<LteEnb> GetMacro ( void ) const;
    
    /**
     * \brief Provide access to the pico eNBs
     * \return <-- Vector of pico eNBs
     */
    gnsm::Vec_t<LteEnb> GetPicos ( void ) const;

    /**
     * \brief Provide the cluster ID
     */
    gnsm::Id_t ReadId ( void ) const;
    
    LteClusterConf const& ReadConf ( void ) const;
    
private:
    gnsm::Id_t m_id;
    
    gnsm::Ptr_t <LteEnb> m_macro;
    gnsm::Vec_t <LteEnb> m_pico;
    const LteClusterConf* m_conf; 

};

#endif /* LTECLUSTER_H */