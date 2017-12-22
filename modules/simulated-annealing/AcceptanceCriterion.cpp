#include "AcceptanceCriterion.h"
#include "Log.h"

namespace SA
{

LOG_REGISTER_MODULE ( "SaAcceptance" );

AcceptanceCriterion::AcceptanceCriterion ( )
:m_gen ( std::random_device ( )( ) ),
m_uniform ( std::uniform_real_distribution<double> ( 0.0, 1.0) )
{
    BEG
    END
}

AcceptanceCriterion::~AcceptanceCriterion ( )
{
    BEG END
}

void
AcceptanceCriterion::Reset ( double temperature, double k )
{
    BEG;
    m_temperature = temperature;
    m_k = k;
    END;
}

bool
AcceptanceCriterion::Accept ( double delta )
{
    BEG;
    bool ret_;
    if ( delta < 0.0 )
    {
        ret_ = true;
    }
    else 
    {
        ret_ =  (m_uniform(m_gen) <= std::exp (- std::fabs(delta) / m_temperature ));
    }
    END;
    return ret_;
}

}// namespace SA