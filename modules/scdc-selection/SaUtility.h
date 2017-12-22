#ifndef SAUTILITY_H
#define SAUTILITY_H


#include "InfoCarrier.h"
#include "ScdcUtils.h"

class SaUtility
{
public:

    SaUtility(void);
    ~SaUtility();
    /**
     * \brief Set the network state to calculate the utility upon a given assignment 
     * and allocation
     * \param users --> Users
     * \param inter --> Set of interfering cells
     * \return --
     */
    void SetNetState(TranslatedVec_t const&);

    /**
     * \brief Calculate the utility for the given allocation. 
     * It also updates the throughput excess 
     * \param info --> Allocation information
     * \return <-- Utility
     */
    double Cost(InfoCarrier& info);

private:

    double DoCost(InfoCarrier& ic);


    /**
     * \brief Give the throughput achieved by an assignment and its spectral efficiency
     * \param assign --> Resources assigned
     * \param pow --> Rx power 
     * \param noise --> Noise, it includes the only-interfering cells
     * \param inter --> Mutual interference
     * \return <-- Pair with the thput and the Seff
     */
    std::pair<double, double> GetPerformance(double assign, double pow,
                                             double noise, double inter) const;

    TranslatedVec_t const* m_trVec;
};

#endif /* SAUTILITY_H */