#ifndef LTEUE_H
#define LTEUE_H

#include "CoreUtils.h"
#include "LteUeConf.h"
#include "LteUtils.h"
#include "../lte-ae/LteCell.h"
#include "Sinr.h"
#include "Traffic.h"
#include "Power.h"

class LteUe
{
public:
    using CellsList_t = gnsm::Vec_t<LteCell>;

    struct CellScan
    {
        CellScan(gnsm::Ptr_t<LteCell> cell = nullptr,
                 Power rsrp = Power(units::Watt(0.0)),
                 EnbType type = EnbType::NONE)
        : m_cell(cell)
        , m_rsrp(rsrp)
        , m_type(type)
        {
        }
        
        gnsm::Ptr_t<LteCell> m_cell;
        Power m_rsrp;
        EnbType m_type;
    };

    struct UlLosses
    {

        UlLosses(gnsm::Ptr_t<LteCell> cell = nullptr,
                 units::dB pl = units::dB(0.0),
                 EnbType type = EnbType::NONE)
        : m_cell(cell)
        , m_pl(pl)
        , m_type(type)
        {
        }

        gnsm::Ptr_t<LteCell> m_cell;
        units::dB m_pl;
        EnbType m_type;
    };

    using SensedValues_t = std::vector<CellScan>; // DL RSRP sensing
    using UlEstimate_t = std::vector<UlLosses>; // for the UL control power

    struct DlConn
    {
        Traffic m_traffic;
        double m_rbs;
    };

    struct UlConn
    {
        Power m_power;
        double m_rbs;
    };

    explicit LteUe(gnsm::Id_t id);
    void SetConfiguration(LteUeConf const& conf);
    LteUeConf const& ReadConfiguration(void);

    /**
     * \brief Store the power sensed from an specific cell 
     * \param cell --> Pointer to cell
     * \param rxPower --> Received power
     * \param ulPl --> Pathloss between the UE and the cell for the UL control power
     * \param type --> Type of the eNB holding the cell
     * \return --
     */
    void AddCellInfo(gnsm::Ptr_t<LteCell> cell, Power rsrp, units::dB ulPl, EnbType type);

    /**
     * \brief Provide a list of the cells scanned ordered by the RSRP, the first the highest RSRP
     * \return <-- Ordered vector of cells
     */
    SensedValues_t const& ReadOrderedCellsDl(void);

    /**
     * \brief Give DL information of a given cell
     * \param c --> Ptr to the cell
     * \return <-- Cell info. If the cell does not exist the information is empty, so the 
     * ptr to the cell within the information struct is nullptr
     */
    CellScan ReadCellDl(gnsm::Ptr_t<LteCell> const& c);

    /**
     * \brief Give the list of available cells for UL ordered by the lowest path-loss.
     * This PL can be directly used for the control power as it accounts for the Tx and 
     * Rx gains
     * \return <-- List of cells
     */
    UlEstimate_t const& ReadOrderedCellsUl(void);

    /**
     * \brief Give UL information of a given cell
     * \param --> Ptr to the cell
     * \return <-- Cell info. If the cell does not exist the information is empty, so the 
     * ptr to the cell within the information struct is nullptr
     */
    UlLosses ReadCellUl(gnsm::Ptr_t<LteCell> const& c);

    /**
     * \brief Perform user connection for down-link
     * \param cl --> List of cells that the user is connected to
     * \param dlc --> Downlink connection information
     * \return --
     */
    void DlConnect(CellsList_t cl, DlConn dlc);

    /**
     * \brief Perform user connection for up-link
     * \param cl --> List of cells that the user is connected to
     * \return -- 
     */
    void UlConnect(CellsList_t cl, UlConn ulc);

    /**
     * \brief Provide current DL connection list
     * \return <-- List of cells
     */
    CellsList_t const& ReadDlConnList(void) const;

    /**
     * \brief Give information of the DL connection
     * \return <-- DL conn info
     */
    DlConn const& ReadDlConnInfo(void) const;

    /**
     * \brief Give list of previous DL cells
     * \return <-- DL cells
     */
    CellsList_t const& ReadPrevDlConnList(void) const;

    /**
     * \brief Provide current UL connection list
     * \return <-- List of cells
     */
    CellsList_t const& ReadUlConnList(void) const;

    /**
     * \brief Give information about the UL connection
     * \return <-- UL conn info
     */
    UlConn const& ReadUlConnInfo(void) const;

    /**
     * \brief Give the list of UL cells
     * \return <-- List of cells
     */
    CellsList_t const& ReadPrevUlConnList(void) const;

    /**
     * \brief Remove the list of cells of the connection
     * \return -- 
     */
    void CallUp(void);

private:
    // Information about the DL/UL connection - likely to be completed
    DlConn m_dlConn;
    UlConn m_ulConn;

    // List of cells the user is connected to
    CellsList_t m_dlCells;
    CellsList_t m_ulCells;

    // List of cells the user was connected to in the previous snapshot
    CellsList_t m_prevDlCells;
    CellsList_t m_prevUlCells;

    // local variables to order the cells
    bool m_orderedDl;
    bool m_orderedUl;

    /**
     * Values sensed for DL and UL:
     * Rx power per RB received by the UE or the cell
     */
    SensedValues_t m_sensedDlPower;
    UlEstimate_t m_ulEstimates;

    gnsm::Id_t m_id;
    const LteUeConf* m_conf;
};

#endif /* LTEUE_H */

