#ifndef LTECELL_H
#define LTECELL_H

#include "CoreUtils.h"
#include "LteCellConf.h"

class LteEnb;
class LteCell {
public:
    using UesList_t = std::vector<gnsm::Id_t>;

    LteCell(gnsm::Id_t id);

    void SetConfiguration(LteCellConf const& conf);
    gnsm::Id_t const& GetId(void) const;

    /**
     * \brief Provide reading access to the configuration
     * \return <-- Const reference to the configuration class
     */
    LteCellConf const& GetConfiguration(void) const;

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

    
    double GetDlResources(void) const;
    double GetUlResources(void) const;
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
    void SetEnb(LteEnb* enbId);

    /**
     * \brief Get eNB ref
     */
    LteEnb* GetEnb(void);
    
    /**
     * \brief Check whether the passed cell belongs to the eNB
     */
    bool HasSameEnb (gnsm::Ptr_t<LteCell> other) const;

private:

    LteEnb* m_enb;
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

