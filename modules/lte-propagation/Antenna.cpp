#include <complex>

#include "Log.h"
#include "Antenna.h"
#include "Position.h"
LOG_REGISTER_MODULE("Antenna");

namespace LTE
{
constexpr double m_etilt = 15.0;
constexpr double m_theta3dB = 10.0;
constexpr double m_slaV = 20.0;
constexpr double m_phi3dB = 60.0;
constexpr double m_am = 25.0;

double
ComputeTheta(Position const& bs, Position const& ue)
{
    BEG;
    //    MSG_ASSERT(GetZDelta(bs, ue) >= 0, "Base station is below user equipment ¿?");
    auto theta_ = std::atan2(GetZDeltaAbs(bs, ue).GetM(), GetPlanarDistance(bs, ue).GetM())
            * 180 / M_PI;
    END;
    return theta_;
}

double
ComputePhi(double azimut, Position const& bs, Position const& ue)
{
    BEG;

    auto val_ = std::atan2(GetXDelta(ue, bs).GetM(), GetYDelta(ue, bs).GetM()) * 180 / M_PI;
    auto phi_ = val_ - azimut;
    if (phi_ < -180)
    {
        phi_ = 360 + phi_;
    }
    END;
    return phi_;
}

double
VerticalPattern(double theta)
{
    BEG END;
    return -std::min(12.0 * std::pow((std::abs(theta) - m_etilt) / m_theta3dB, 2), m_slaV);
}

double
HorizontalPattern(double phi)
{
    BEG END;
    return -std::min(12.0 * std::pow(std::abs(phi) / m_phi3dB, 2), m_am);
}

/**************************************************************************************************/


units::dB
ComputeAntennaAtt(double azimut, Position const& bs, Position const& ue)
{
    BEG;
    auto theta_ = ComputeTheta(bs, ue);
    auto phi_ = ComputePhi(azimut, bs, ue);
    INFO("Value of theta angle (V) ", theta_, "º");
    INFO("Value of phi angle (H) ", phi_, "º");
    END;
    return units::dB(std::min(-(HorizontalPattern(phi_) + VerticalPattern(theta_)), m_am));
}

units::dB
ComputeAntennaAtt_Honly(double azimut, Position const& bs, Position const& ue)
{
    BEG;
    auto phi_ = ComputePhi(azimut, bs, ue);
    END;
    return units::dB(std::min(-HorizontalPattern(phi_), m_am));
}

} // namesapce LTE