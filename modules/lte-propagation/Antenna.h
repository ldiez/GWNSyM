#ifndef TRIANTENNA_H
#define	TRIANTENNA_H

#include "Position.h"
#include "Power.h"
namespace LTE
{

/**
 * \ingroup LTE 
 * \brief Implementation of antenna pattern. It computes the antenna gain according to
 * the user-eNB position in 3D.
 * 
 * This function implements the antenna pattern used for LTE simulation models, defined in TR.36.814:
 * 
 * - Vertical antenna pattern
 * G_V[dB](theta) = -min [ 12 ((theta - theta_etilt)/(theta_3dB))^2, SLA_v]
 * where theta_3dB = 10, SLA_v = 20dB, theta_etilt = 15º for ISD = 500m$ and 
 * theta_etilt = 6º for ISD = 1732m (UE at 1.5 m)
 * 
 * - Horizontal antenna pattern
 * G_H[dB](phi) = -min [ 12 (phi/phi_3dB)^2, A_m]
 * where phi_3dB = 70º, A_m = 25dB
 * 
 * - Combining method for 3D antenna pattern
 * A_(phi, theta) = -min [-[A_H(phi) + A_V(theta)], A_m]
 * 
 * The north is assumed to be towards te possitive Y direction. 
 */

/**
 * \brief Compute the gain according to the antenna pattern and the relative positions of the 
 * base station and user equipment
 * \param azimut --> angle in degrees
 * \param bs --> Base station position
 * \param ue --> User equipment position
 * \return <-- Antenna gain in dBs
 */
units::dB ComputeAntennaAtt ( double azimut, Position const& bs, Position const& ue );

units::dB ComputeAntennaAtt_Honly ( double azimut, Position const& bs, Position const& ue );

} // namespace LTE
#endif	/* TRIANTENNA_H */