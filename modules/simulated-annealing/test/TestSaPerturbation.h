#ifndef TESTSAPERTURBATION_H
#define	TESTSAPERTURBATION_H

#include <iostream>
#include <algorithm>
#include "Perturbation.h"
#include "gnuplot_i.h"

namespace SA
{

inline void TestSaPerturbation ( void )
{
    //         set type of distribution to use
    SA::Perturbation* dist_ = new SA::Perturbation();
    dist_->SetType(SA::PertType_e::CAUCHY);
    // set temperature value 
    Gnuplot gp_;
    gp_.set_style("linespoints").set_yrange(0, 1.0);

    // calculate an histogram
    const double min = -100.0;
    const double max = 100.0;
    const double step = 1;

    for ( double T = 1.0; T <= 1.0; T += 1.0 )
    {
        dist_->Reset(T);
        std::vector<double> myVect_;
        for ( int i = 0; i < 100; ++i )
        {
            myVect_.push_back(dist_->GetValue());
        }
        std::vector<double > hist_(std::round(( max - min ) / step) + 1);
        std::uint32_t count(0);
        std::vector<float> xAxis_;
        for ( float i = min; i <= max; i += step )
        {
            int curr_ = min + step*count;
            xAxis_.push_back(curr_ + step / 2);
            hist_.at(count) = std::count_if(myVect_.begin(), myVect_.end(), [curr_, step]
                                            (double i)
            {
                return ( i > curr_ && i < curr_ + step );
            });
            ++count;
        }
        for ( uint32_t it = 0; it < hist_.size(); ++it )
        {
            hist_.at(it) /= myVect_.size();
        }
        gp_.plot_xy(xAxis_, hist_);
    }

    wait_for_key();

    delete dist_;
}

} // namespace SA
#endif	/* TESTSAPERTURBATION_H */