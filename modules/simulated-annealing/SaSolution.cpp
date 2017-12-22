#include <cmath>

#include "Log.h"
#include "SaSolution.h"

namespace SA
{

LOG_REGISTER_MODULE ( "SaSolution" )

SaSolution::SaSolution ( ) 
: m_error (std::numeric_limits<double>::max())
, m_auxError(std::numeric_limits<double>::max())
{
    BEG END;
}

double
SaSolution::Initialize ( void )
{
    BEG END;
    DoInitialize ();
    m_error = ComputeError ();
    return m_error;
}

double
SaSolution::TryPerturb ( double temp, double initTemp, Perturbation& pert )
{
    BEG END;
    DoTryPerturb (temp, initTemp, pert);
    m_auxError = ComputeAuxError ();
    return m_auxError;
}

void 
SaSolution::ApprovePerturb ( void )
{
    BEG END;
    DoApprovePerturb ();
    m_error = m_auxError;
}

double
SaSolution::GetError ( void )
{
    BEG END;
    return m_error;
}

void 
SaSolution::PlotResults ( void )
{
    BEG;
    DoPlotResults ();
    END;
}

} // namespace SA