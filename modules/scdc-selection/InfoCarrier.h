#ifndef INFOCARRIER_H
#define INFOCARRIER_H

#include <vector>
#include <cstdint>

/**
 * \brief This class carries the information among the different components of simulated
 * annealing
 */
class InfoCarrier
{
public:
    InfoCarrier();
    ~InfoCarrier();
    
     /**
     * \brief Give the system factor (F) by which the number of resources are multiplied 
     * to get Hz, so that:
     * 
     * Thput = Nr * F * Seff;
     * 
     * It is the resources bandwidth and the effective bandwidth
     */
    static const double ResFactor;
    
    struct Unit
    {
        const double m_targetThput;
        double m_thputExcess;
        double m_current;
        double m_seff;
        double m_mutualInter; 
    };

    /**
     * \brief Create a new element with its target throughput and:
     * + m_thputExcess = -m_targetThput
     * + m_current = 0.0
     * + m_seff = 0.0
     * \param target --> Target throughput
     * \return --
     */
    void AddUnit(double target);

    /**
     * \brief Give the number of elements
     */
    std::uint32_t GetNumUnits(void) const;

    /**
     * \brief Give the current assignment
     * \param n --> Index of the element
     * \return <-- Current assignment
     */
    double GetAssignment(std::uint32_t n) const;

    /**
     * \brief Give the spectral efficiency
     * \param n --> Index of the element
     * \return <-- Seff
     */
    double GetSeff(std::uint32_t n) const;

    /**
     * \brief Give the target throughput
     * \param n --> Element index
     * \return <-- Target thput
     */
    double GetTarget(std::uint32_t n) const;

    /**
     * \brief Give the current throughput excess
     * \param n --> Index of the element
     * \return <-- Thput exces
     */
    double GetThputExcess(std::uint32_t n) const;
    
    /**
     * \brief Give the amount of mutual interference
     * \param n --> Index of the element
     * \return <-- Mutual interference
     */
    double GetMutualInter(std::uint32_t n) const;

    /**
     * \brief Set current assignment
     * \param n --> Index of the element
     * \param curr --> New assignment
     * \return --
     */
    void SetAssignment(std::uint32_t n, double curr);

    /**
     * \brief Set spectral efficiency
     * \param n --> Index of the element
     * \param seff --> New SE
     * \return --
     */
    void SetSeff(std::uint32_t n, double seff);

    /**
     * \brief Set throughput excess
     * \param n --> Index of the element
     * \param excess --> Throughput excess
     * \return --
     */
    void SetThputExcess(std::uint32_t n, double excess);
    
    /**
     * \brief Set the amount of mutual interference
     * \param n --> Index of the element
     * \param inter --> Mutual interference
     * \return --
     */
    void SetMutualInter(std::uint32_t n, double inter);
    
    /**
     * \brief Remove local information
     * \return --
     */
    void Clear( void );

private:
    std::vector<Unit> m_units;
};

#endif /* INFOCARRIER_H */

