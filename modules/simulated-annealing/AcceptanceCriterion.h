#ifndef ACCEPTANCECRITERION_H
#define	ACCEPTANCECRITERION_H

#include <random>

namespace SA
{

/**
 * \ingroup SA
 * \brief This class implements different acceptance criteria for a simulated annealing algorithm.
 */
class AcceptanceCriterion
{
public:
    AcceptanceCriterion ( );
    ~AcceptanceCriterion ( );

    /**
     * \brief Configure the acceptence criterion by setting the current temperature
     * \param temperature --> Current temperature
     * \return -- 
     */
    void Reset ( double temperature, double k = 1.0 );

    /**
     * \brief Check if a new energy is accepted or not based in the previous value. A new energy
     * is accepted if it is lower than the current energy (better), otherwise it is accepted with 
     * some probability 
     * \param delta --> New energy
     * \return <-- True of the new energy is acepted, false otherwise
     */
    bool Accept ( double delta );


private:
    double m_temperature = 0.0;

    std::mt19937 m_gen;
    std::uniform_real_distribution<double> m_uniform;
    double m_k;
};

} // namespace SA
#endif	/* ACCEPTANCECRITERION_H */