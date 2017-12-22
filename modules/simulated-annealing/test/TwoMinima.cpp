#include <random>
#include <queue>
#include <algorithm>

#include "Log.h"
#include "TwoMinima.h"
#include "gnuplot_i.h"

namespace SA
{

LOG_REGISTER_MODULE ( "SaTwoMinima" );

TwoMinima::TwoMinima ( )
{
    BEG END;
}

TwoMinima::~TwoMinima ( )
{
    BEG END;
}

void
TwoMinima::InitQueues ( void )
{
    BEG;
    std::queue<double> xEmpty_;
    std::swap (m_xArray, xEmpty_);
    std::queue<double> errorEmpty_;
    std::swap (m_errorArray, errorEmpty_);
    END;
}

void
TwoMinima::DoInitialize ( void )
{
    BEG;
    InitQueues ();

    std::uniform_real_distribution<double> dist_ (-10.0, 10.0);
    m_x = dist_ (m_gen);

    if ( m_xArray.size () > m_queuesSize )
    {
        m_xArray.pop ();
        m_errorArray.pop ();
    }
    m_xArray.push (m_x);
    m_errorArray.push (ComputeError ());
    END;
}

void
TwoMinima::DoTryPerturb ( double temp, double initTemp, Perturbation& pert )
{
    BEG;
    m_auxX = m_x + pert.GetValue ();
    if ( m_auxX < -10.0 )
    {
        m_auxX = 10.0;
    }
    else if ( m_auxX > 10.0 )
    {
        m_auxX = 10.0;
    }
    END;
}

void
TwoMinima::DoApprovePerturb ( void )
{
    BEG;
    m_x = m_auxX;
    if ( m_xArray.size () > m_queuesSize )
    {
        m_xArray.pop ();
        m_errorArray.pop ();
    }
    m_xArray.push (m_x);
    m_errorArray.push (ComputeError ());
    END;
}

double
TwoMinima::ComputeError ( void )
{
    BEG END;
    double aux1_ = 0.01 * std::pow (m_x - 3, 2);
    double aux2_ = std::cos (3 * M_PI / 9 * ( m_x - 1 ));
    return 1 + aux1_ + aux2_;
}

double
TwoMinima::ComputeAuxError ( void )
{
    BEG END;
    double aux1_ = 0.01 * std::pow (m_auxX - 3, 2);
    double aux2_ = std::cos (3 * M_PI / 9 * ( m_auxX - 1 ));
    return 1 + aux1_ + aux2_;
}

void
TwoMinima::DoPlotResults ( void )
{
    BEG;
    std::cout << "X value: " << std::to_string (m_x) << std::endl;
    std::cout << "Objective function: " << m_error << std::endl;

    std::uint32_t xAxisRange_ = ( m_queuesSize > m_xArray.size () ?
            m_xArray.size () : m_queuesSize );

    std::vector<double> xVec_;
    std::vector<double> errorVec_;

    std::uint32_t size_ (m_xArray.size ());
    for ( std::uint32_t it_ = 0; it_ < size_; ++it_ )
    {
        xVec_.push_back (m_xArray.front ());
        errorVec_.push_back (m_errorArray.front ());
        m_xArray.pop ();
        m_errorArray.pop ();
    }

    Gnuplot gp2_;
    gp2_.set_style ("points")
            .set_title ("X variable")
            .set_xrange (0, xAxisRange_)
            .set_grid ()
            .plot_x (xVec_, "X variable");

    Gnuplot gp4_;
    gp4_.set_style ("points")
            .set_title ("Objective")
            .set_xrange (0, xAxisRange_)
            .set_grid ()
            .plot_x (errorVec_, "Objective function");

    wait_for_key ();
    END;
}

} // namespace