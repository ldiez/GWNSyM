#include <cmath>
#include <complex>
#include <algorithm>

#include "CoolingSchedule.h"
#include "Log.h"


namespace SA
{
LOG_REGISTER_MODULE ( "SaCoolingSchedule" );

CoolingSchedule::CoolingSchedule ( double initTemp)
: m_initTemp(initTemp)
{
    BEG END;
}

CoolingSchedule::~CoolingSchedule ( )
{
    BEG END;
}

void
CoolingSchedule::SetCoolingType ( CoolingType_e type )
{
    BEG;
    m_coolingType = type;
    END;
}

void
CoolingSchedule::SetCoolingParameter ( double alpha )
{
    BEG;
    MSG_ASSERT (alpha <= 1.0 && alpha > 0.0, "Set alpha in the range (0, 1.0]");
    m_alpha = alpha;
    END;
}

void
CoolingSchedule::SetSerachSpace ( std::uint32_t ss )
{
    BEG;
    m_searchSpace = ss;
    END;
}

void
CoolingSchedule::SetFinalTemp ( double finalTemp )
{
    BEG;
    MSG_ASSERT (finalTemp < m_initTemp, "Set a final temperature that is less than "
                "the initial temperature!!");
    m_finalTemp = finalTemp;
    END;
}

void 
CoolingSchedule::SetNumTemps (std::uint32_t numTemps)
{
    BEG;
    m_numTemps = numTemps;
    END;
}

double
CoolingSchedule::GetValue ( std::uint32_t iter )
{
    BEG;
    double ret_ = ( m_coolingType == CoolingType_e::EXP ? ExpFunc (iter) :
            m_coolingType == CoolingType_e::LINEAR ? LinearFunc (iter) : 
            m_coolingType == CoolingType_e::GEOM ? GeomFunc (iter) :
            m_coolingType == CoolingType_e::LOG ? LogFunc (iter) : LinearFunc (iter));
    END;
    return ret_;
}

double
CoolingSchedule::LogFunc ( std::uint32_t iter )
{
    BEG END;
    MSG_ASSERT (m_alpha > 0.0, "Cooling parameter not set for logarithmic schedule. Set it!!");
    return (m_alpha * m_initTemp / ( std::log (1 + iter) ));
}

double
CoolingSchedule::GeomFunc ( std::uint32_t iter )
{
    BEG END;
    MSG_ASSERT (m_alpha > 0.0 && m_alpha < 1.0 , "Cooling parameter not properly set for geometric "
            "schedule (0.0,1.0). Set it!!");
    return (std::pow (m_alpha, iter) * m_initTemp );
}

double
CoolingSchedule::ExpFunc ( std::uint32_t iter )
{
    BEG END;
    MSG_ASSERT (m_searchSpace > 0 && m_alpha > 0.0, "Search space has no dimension and/or "
                "cooling parameter not set for exponential schedule. Set it!!");
    return m_initTemp * std::exp (-m_alpha * std::pow (float(iter ), 1.0 / m_searchSpace));
}

double
CoolingSchedule::LinearFunc ( std::uint32_t iter )
{
    BEG END;
    MSG_ASSERT (m_numTemps > 0, "Linear parameters not set. Set it!!");
    return (m_initTemp + iter * ( m_finalTemp - m_initTemp ) / ( m_numTemps ) );
}

} // namespace SA