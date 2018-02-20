#ifndef LTECELL_H
#define LTECELL_H

#include "CoreUtils.h"
#include "LteCellConf.h"

class LteCell {
public:
    using UesList_t = std::vector<gnsm::Id_t>;

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
    bool AddDlUser(gnsm::Id_t eu, double cap);
    bool AddUlUser(gnsm::Id_t u, double cap);

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
    double GetDlFreeResources(void) const;
    double GetUlFreeResources(void) const;

    /**
     * \brief Provide the load of the cell
     * \return <-- Load of the cell relative to 1
     */
    double GetDlLoad(void) const;
    double GetUlLoad(void) const;

    /**
     * \brief Reset the current state to get ready for the next iteration
     * \return --
     */
    void CallUp(void);

    /**
     * \brief Set eNB ref
     */
    void SetEnbId(gnsm::Id_t enbId);

    /**
     * \brief Get eNB ref
     */
    gnsm::Id_t ReadEnbId(void);

private:


    gnsm::Id_t m_id;
    const LteCellConf* m_conf;
    double m_azimut;
    gnsm::Id_t m_enbId;

    double m_dlFreeResources;
    double m_ulFreeResources;
    UesList_t m_ulUes;
    UesList_t m_dlUes;

};

#endif /* LTESCELL_H */

