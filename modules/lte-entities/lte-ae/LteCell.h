#ifndef LTECELL_H
#define LTECELL_H

#include "CoreUtils.h"
#include "LteCellConf.h"

class LteCell
{
public:
    LteCell(gnsm::Id_t id);

    void SetConfiguration(LteCellConf const& conf);
    gnsm::Id_t const& ReadId(void) const;

    /**
     * \brief Provide reading access to the configuration
     * \return <-- Const reference to the configuration class
     */
    LteCellConf const& ReadConfig(void) const;

    /**
     * \brief Connect a new user 
     * \param id --> user identifier
     * \param cap --> Resources required for that user
     * \return <-- True if the connection is accepted, false otherwise
     */
    bool AddUser(double cap);

    /**
     * \brief Set the sector azimut in degrees
     * \param azimut --> Azimut value
     * \return --
     */
    void SetAzimut(double azimut);

    /**
     * \brief Azimut value for this sector
     * \return <-- Angle value in degrees
     */
    double GetAzimut(void) const;

    /**
     * \brief Free resources at the eNB sector in number of RBs
     * \return <-- The number of RBs
     */
    double GetFreeResources(void) const;

    /**
     * \brief Provide the load of the cell
     * \return <-- Load of the cell relative to 1
     */
    double GetLoad(void) const;

    /**
     * \brief Reset the current state to get ready for the next iteration
     * \return --
     */
    void CallUp(void);

private:
    gnsm::Id_t m_id;
    const LteCellConf* m_conf;
    double m_azimut;
    double m_freeResources;
    std::uint32_t m_nConn;
};

#endif /* LTESCELL_H */

