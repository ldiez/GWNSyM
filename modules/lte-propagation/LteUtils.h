#ifndef LTEUTILS_H
#define	LTEUTILS_H

#include <ostream>
#include <limits>
#include <c++/5/limits>
#include "Sinr.h"
#include "Bandwidth.h"

/**
 * \ingroup LTE
 * \brief Definition of basic constants to calculate SINR.
 * It assumes resource-blocks of 180 KHz
 */
namespace LTE
{
// resource block
static const units::kHz RbBw_s(180.0);
static constexpr double BwEff_s = 0.85;

static constexpr double SinrEff_s = 1.25;
static constexpr double PreCod_s = 0.5;
static constexpr double SelfPreCod_s = 0.1;
static constexpr double SelfInterFactor_s = LTE::SinrEff_s * LTE::SelfPreCod_s;
static constexpr double InterFactor_s = LTE::SinrEff_s * LTE::PreCod_s;

static constexpr double RbAwgnMw_s = NoisePowerWatt(180000)*1000; // Noise power of 1 PRB in mW
static constexpr double RbAwgnMwEff_s = LTE::SinrEff_s * RbAwgnMw_s;

static constexpr double UlAlpha_s = 1.0;
static const units::dB UlIm_s(2.0);

// outage SINR 
static const units::dB OutageSinr_s (0.0);


// handy extremely low/high
static const auto TooLow_s = -std::numeric_limits<double>::max();
static const auto TooHigh_s = std::numeric_limits<double>::max();
static const units::dB TooLowPower_s (-1.0e6);
static const units::dB TooLowSinr_s (-1.0e6);

} // namespace LTE

#endif	/* LTEUTILS_H */