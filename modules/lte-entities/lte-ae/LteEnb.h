#ifndef LTEENB_H
#define	LTEENB_H

#include "CoreUtils.h"
#include "Position.h"

#include "LteEnbConf.h"
#include "LteCell.h"

#include "Wrapper.h"

class LteEnb
{
public:
    LteEnb ( gnsm::Id_t id );

    /**
     * \brief System interface
     */
    void SetConfiguration ( LteEnbConf const& conf);
    LteEnbConf const& ReadConf ( void ) const;
    void Aggregate ( std::string const& name, gnsm::ts::Wrapper_t o );
    
    /**
     * \brief Generic stuff
     */
    gnsm::Id_t ReadId ( void ) const;
    void SetPosition ( Position pos );
    Position const& ReadPosition ( void ) const;
    
    /**
     * \brief Provide access to the sectors. The return value can be copied.
     * \return <-- Vector of sectors
     */
    gnsm::Vec_t<LteCell> const& ReadCells ( void ) const;
    
    /**
     * \brief Get a sector by ID
     * \param sectorId --> Sector ID
     * \return <-- Pointer to the sector
     */
    gnsm::Ptr_t<LteCell> GetCell ( gnsm::Id_t sectorId) const;
    
    /**
     * \brief Prepare the instance for a new iteration
     */
    void CallUp ( void );
    
    
    
private:
    gnsm::Vec_t<LteCell> m_cells;
    const LteEnbConf*  m_conf;
    const gnsm::Id_t m_id;
    Position m_pos;
};

#endif	/* LTEENB_H */

