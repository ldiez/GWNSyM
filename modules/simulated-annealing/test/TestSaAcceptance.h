#ifndef TESTSAACCEPTANCE_H
#define	TESTSAACCEPTANCE_H

#include <iostream>
#include <algorithm>
#include "AcceptanceCriterion.h"
#include "gnuplot_i.h"

namespace SA
{

inline void TestSaAcceptance ( void )
{
    //         set type of distribution to use
    SA::AcceptanceCriterion* accCrit_ = new SA::AcceptanceCriterion ( );
    delete accCrit_;
}

} // namespace SA

#endif	/* TESTSAACCEPTANCE_H */