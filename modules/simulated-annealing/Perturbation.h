#ifndef      PERTURBATION_H
#define     PERTURBATION_H

#include <random>
#include <cmath>
#include <iostream>
#include <ostream>

namespace SA
{

/**
 * \ingroup SA
 * \brief This class implements different random distributions to be used by 
 * simmulated annealing algorithms in the perturbation phase
 * 
 * The class will provide uniform, normal (Gaussian), and Cauchy distributions for different
 * temperatures of the annealing process.
 * 
 * For the time being only integer values are provided, since the assigned resources are 
 * also integer. 
 */

enum class PertType_e : std::uint8_t
{
    UNIFORM,
    NORMAL,
    CAUCHY
};

class Perturbation
{
public:
    Perturbation (  );
    ~Perturbation ( );
    
    /**
     * \brief Set perturbation type (distribution)
     * \param type --> Type
     * \return --
     */
    void SetType ( PertType_e type );
    
    /**
     * \brief Reset temperature parameter so that the used distribution is reset                   
     * accordingly
     * \param sf --> Scaling factor
     * \return <-- void
     */
    void Reset ( double sf );

    /**
     * \brief Obtains a new value according to the current settings
     * \return <-- Real value
     */
    double GetValue ( void );
    
    /**
     * \brief Return the least upper bound for the distribution
     * \return <-- Least upper bound value
     */
    double GetMax ( void );
    
    /**
     * \brief Return the greatest lower bound for the distribution
     * \return <-- Greatest lower bound
     */
    double GetMin ( void );

    inline friend std::ostream& operator<< ( std::ostream& out, const Perturbation& pert );
private:
    std::default_random_engine m_gen;
    PertType_e m_type;

    std::uniform_real_distribution<double> m_uniform;
    std::normal_distribution<double> m_normal;
    std::cauchy_distribution<double> m_cauchy;
    
    double m_sf;
    bool m_set = false;
};

std::ostream&
operator<< ( std::ostream& out, const Perturbation& pert )
{
    out << "Perturbation model with random distribution "
            << ( pert.m_type == PertType_e::NORMAL ? "NORMAL" :
            pert.m_type == PertType_e::UNIFORM ? "UNIFORM" :
            "CAUCHY" );
    return out;
}

} // namespace SA
#endif	/* PERTURBATION_H */