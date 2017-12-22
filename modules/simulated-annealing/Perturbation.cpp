#include "Perturbation.h"

#include "Log.h"

namespace SA
{

LOG_REGISTER_MODULE ( "SaPerturbation" );

Perturbation::Perturbation ( )
: m_gen ( std::random_device ( )( ) )
{
    BEG END
}

Perturbation::~Perturbation ( )
{
    BEG END
}

void
Perturbation::SetType ( PertType_e type )
{
    BEG;
    m_type = type;
    END;
}

void
Perturbation::Reset ( double sf )
{
    BEG;
    m_set = true;
    if ( m_type == PertType_e::NORMAL )
    {
        m_normal = std::normal_distribution<double> ( 0, sf );
    }
    else if ( m_type == PertType_e::CAUCHY )
    {
        DBG ("Setting cauchy distribution with spread factor ", sf);
        m_cauchy = std::cauchy_distribution<double>( 0.0, sf );
    }
    else
    {
        m_uniform = std::uniform_real_distribution<double> ( -sf, sf );
    }
    END;
}

double
Perturbation::GetValue ( void )
{
    BEG;
    MSG_ASSERT(m_set, "No settings found, please call reset with the proper "
            "temperature value.");
    END;
    return (m_type == PertType_e::CAUCHY ? m_cauchy(m_gen) :
            m_type == PertType_e::NORMAL ? m_normal(m_gen) :
            m_uniform(m_gen) );
}

double
Perturbation::GetMax ( void )
{
    BEG END;
    MSG_ASSERT(m_set, "No settings found, please call reset with the proper "
            "temperature value.");
    return ( m_type == PertType_e::CAUCHY ? m_cauchy.max() :
            m_type == PertType_e::NORMAL ? m_normal.max() :
            m_uniform.max() );
}

double
Perturbation::GetMin ( void )
{
    BEG END;
    MSG_ASSERT(m_set, "No settings found, please call reset with the proper "
            "temperature value.");
    return ( m_type == PertType_e::CAUCHY ? m_cauchy.min() :
            m_type == PertType_e::NORMAL ? m_normal.min() :
            m_uniform.min() );
}
} // namespace SA

