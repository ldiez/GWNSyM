#ifndef LTECLUSTERLOCATORSQUARE_H
#define LTECLUSTERLOCATORSQUARE_H

#include "LteCluster.h"
#include "Distance.h"
#include <random>
/**
 * \brief This class locates clusters of eNBs. 
 * The pico cells will be deployed randomly around a macro cell
 */
class LteClusterLocatorSquare
{
public:
    /**
     * \brief Ctor. It creates an hexagonal grid for the clusters and 
     * the pico cells are randomly deployed around a macro in a defined ring.
     * It is ensured that the first cluster is in the center of the lattice
     * \param side --> Side of the scenario to start the grid
     * \param isd --> inter-site-distance: side of the hexagon
     * \param maxRange --> outer range of the pico-square
     * \param minRange --> inner range of the pico-square
     */
    LteClusterLocatorSquare(units::m side, units::m isd, std::uint32_t rings, units::m maxSide,
                      units::m minSide = units::m(0.0));

    void operator()(gnsm::Vec_t<LteCluster> cl);


private:

    /**
     * \brief Deploy the macro-cell
     */
    void DeployMacro(gnsm::Ptr_t<LteCluster> cl);

    /**
     * \brief Deploy the pico-cells randomly
     */
    void DeployPicos(gnsm::Ptr_t<LteCluster> cl);

    /**
     * \brief Create radom position within an annulus
     */
    Position GetRandomSide(Position center, int i);

    /**
     * \brief Create the map of coordinates to differentiate between rings
     */
    void BuildCoormap(void);

    /**
     * \brief Pre-load positions of 100 clusters
     */
    void HexPosition(void);
    
    // local store for the access elements positions
    const units::m m_side;
    const units::m m_isd;
    const std::uint32_t m_rings;

    // for pico cells
    const units::m m_maxRange;
    const units::m m_minRange;
    std::uniform_real_distribution<double> m_distSide;
    std::uniform_real_distribution<double> m_distSide2;
    std::default_random_engine m_re;
    std::uint32_t m_ctr;
    std::vector<std::pair<double, double>> m_coord;
};

#endif /* LTECLUSTERLOCATORSQUARE_H */

