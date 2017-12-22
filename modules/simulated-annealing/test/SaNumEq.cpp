#include <utility>

#include "Log.h"
#include "SaNumEq.h"
#include "gnuplot_i.h"

namespace SA
{

LOG_REGISTER_MODULE ( "SaNumEq" );

SaNumEq::SaNumEq ( )
: m_gen ( std::random_device ( )( ) )
{
    BEG END;
}

SaNumEq::~SaNumEq ( )
{
    BEG END;
}

void
SaNumEq::DoInitialize ( void )
{
    BEG;
    std::queue<double> xEmpty_;
    std::swap (m_xArray, xEmpty_);
    std::queue<double> yEmpty_;
    std::swap (m_yArray, yEmpty_);
    std::queue<double> errorEmpty_;
    std::swap (m_errorArray, errorEmpty_);


    std::uniform_real_distribution<double> dist_ (-10.0, 10.0);
    m_x = dist_ (m_gen);
    m_y = dist_ (m_gen);

    if ( m_xArray.size () > m_queuesSize )
    {
        m_xArray.pop ();
        m_yArray.pop ();
        m_errorArray.pop ();
    }
    m_xArray.push (m_x);
    m_yArray.push (m_y);
    m_errorArray.push (ComputeError ());
    END;
}

void
SaNumEq::DoTryPerturb ( double temp, double initTemp, Perturbation& pert )
{
    BEG;
    double ratio_ = temp / initTemp;
    m_auxX = ( 1.0 - ratio_ ) * m_x + ratio_ * pert.GetValue ();
    m_auxY = ( 1.0 - ratio_ ) * m_y + ratio_ * pert.GetValue ();
    END;
}

void
SaNumEq::DoApprovePerturb ( void )
{
    BEG;
    m_x = m_auxX;
    m_y = m_auxY;

    if ( m_xArray.size () > m_queuesSize )
    {
        m_xArray.pop ();
        m_yArray.pop ();
        m_errorArray.pop ();
    }
    m_xArray.push (m_x);
    m_yArray.push (m_y);
    m_errorArray.push (ComputeError ());
    END;
}

double
SaNumEq::ComputeError ( void )
{
    BEG END;
    double res_ = std::pow (m_x, 2) + std::pow (m_y, 2) +
            5.0 * std::fabs (m_x) * std::fabs (m_y) - 4.0;
    return res_;
}

double
SaNumEq::ComputeAuxError ( void )
{
    BEG END;
    double res_ = std::pow (m_auxX, 2) + std::pow (m_auxY, 2) +
            5.0 * std::fabs (m_auxX) * std::fabs (m_auxY) - 4.0;
    return res_;
}

void
SaNumEq::DoPlotResults ( void )
{
    BEG;
    std::cout << "X value: " << std::to_string (m_x) << std::endl;
    std::cout << "Y value: " << std::to_string (m_y) << std::endl;
    std::cout << "Objective function: " << m_error << std::endl;

    std::uint32_t xAxisRange_ = ( m_queuesSize > m_xArray.size () ?
            m_xArray.size () : m_queuesSize );

    std::vector<double> xVec_;
    std::vector<double> yVec_;
    std::vector<double> errorVec_;

    std::uint32_t size_ (m_xArray.size ());
    for ( std::uint32_t it_ = 0; it_ < size_; ++it_ )
    {
        xVec_.push_back (m_xArray.front ());
        yVec_.push_back (m_yArray.front ());
        errorVec_.push_back (m_errorArray.front ());
        m_xArray.pop ();
        m_yArray.pop ();
        m_errorArray.pop ();
    }

    //    Gnuplot gp_;
    //    gp_.set_style ("points")
    //            .set_title ("Both variables")
    //            .set_yrange (-10, 10)
    //            .set_xrange (-10, 10)
    //            .set_grid ()
    //            .plot_xy (m_xArray, m_yArray, "Both variables");


    Gnuplot gp2_;
    gp2_.set_style ("points")
            .set_title ("X variable")
            .set_xrange (0, xAxisRange_)
            .set_grid ()
            .plot_x (xVec_, "X variable");

    Gnuplot gp3_;
    gp3_.set_style ("points")
            .set_title ("Y variable")
            .set_xrange (0, xAxisRange_)
            .set_grid ()
            .plot_x (yVec_, "Y variable");

    Gnuplot gp4_;
    gp4_.set_style ("points")
            .set_title ("Objective")
            .set_xrange (0, xAxisRange_)
            .set_grid ()
            .plot_x (errorVec_, "Objective function");

    wait_for_key ();
    END;
}

} // namespace SA
