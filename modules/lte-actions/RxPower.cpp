#include "RxPower.h"
#include "PathLoss.h"
#include "Antenna.h"
#include "Log.h"
#include "ThreadPool.h"
#include "Sinr.h"

LOG_REGISTER_MODULE("RxPower");

using namespace LTE;

// cable losses in dB
static const units::dB CouplingLoss_c(2.0); // cable
static const units::dB Mcl_c(70.0);

units::dB
ComputePathloss(EnbType type, gnsm::Ptr_t<User> user, gnsm::Ptr_t<LteEnb> ae, PropType_e prop)
{
    BEG;

    auto hbs_ = units::m(ae->ReadPosition().GetZ());
    auto hut_ = units::m(user->ReadPosition().GetZ());
    auto frec_ = ae->ReadConf().GetFrequency();
    auto distance_ = GetPlanarDistance(user->ReadPosition(), ae->ReadPosition()).GetUnitsM();
    INFO("Distance ", distance_.RawVal());
    END;
    if (type == EnbType::MACRO)
    {
        INFO("Computing MACRO pathloss with distance ", distance_.RawVal(), "m");
        return (prop == PropType_e::FULL ?
                UmaPathloss(distance_, frec_, hbs_, hut_) :
                prop == PropType_e::LOS ?
                UmaPathloss_LOS(distance_, frec_, hbs_, hut_) :
                UmaPathloss_NLOS(distance_, frec_, hbs_, hut_));
    }
    else if (type == EnbType::MICRO)
    {
        INFO("Computing MICRO pathloss with distance ", distance_.RawVal(), "m");
        return (prop == PropType_e::FULL ?
                UmiPathloss(distance_, frec_, hbs_, hut_) :
                prop == PropType_e::LOS ?
                UmiPathloss_LOS(distance_, frec_, hbs_, hut_) :
                UmiPathloss_NLOS(distance_, frec_, hbs_, hut_));
    }
    INFO("Computing PICO pathloss with distance ", distance_.RawVal(), "m");
    return (prop == PropType_e::FULL ?
            UpiPathloss(distance_, frec_, hbs_, hut_) :
            prop == PropType_e::LOS ?
            UpiPathloss_LOS(distance_, frec_, hbs_, hut_) :
            UpiPathloss_NLOS(distance_, frec_, hbs_, hut_));
}

units::dB
ComputeAntenna(gnsm::Ptr_t<LteCell> cell, Position const& enbPos, Position const& uPos, AntennaType_e ant)
{
    if (cell->ReadConfig().GetType() != EnbType::MACRO)
    {
        return units::dB(0.0);
    }
    return (ant == AntennaType_e::HONLY ?
            ComputeAntennaAtt_Honly(cell->GetAzimut(), enbPos, uPos) :
            ComputeAntennaAtt(cell->GetAzimut(), enbPos, uPos));
}

/**
 * DOWNLINK BUDGET
 * 
 * - Transmitter
 * EIRP(dBm) = Tx_pow(dBm) + Tx_gain(dBi) - Cable_loss(dB) 
 * 
 * - Receiver
 * Receiver_noise_floor(dBm) = UE_NF(dB) + Th_noise(dBm)
 * 
 * UPLINK BUDGET
 * 
 * - Transmitter
 * EIRP = Tx_pow(dBm) + Tx_gain(dBi)
 * 
 * - Receiver
 * 
 * 
 */


void
LteRxPower(gnsm::Ptr_t<User> user, gnsm::Ptr_t<LteEnb> ae, AntennaType_e ant,
           PropType_e prop)
{
    BEG;
    auto lteUe_ = user->GetLteDev();
    auto gRx_ = lteUe_->ReadConfiguration().ReadRxGain();

    auto pl_ = ComputePathloss(ae->ReadConf().GetType(), user, ae, prop);
    INFO("Pathloss ", pl_.RawVal());

    for (auto& cell_ : ae->ReadCells())
    {
        auto gtx_ = cell_->ReadConfig().GetTxGain();
        auto antennaAtt_ = ComputeAntenna(cell_, ae->ReadPosition(), user->ReadPosition(), ant);
        auto power_ = cell_->ReadConfig().GetTxpowerPerRb();
        auto totalAtt_ = pl_ + CouplingLoss_c + antennaAtt_ - gRx_ - gtx_;
        auto applicablePl_ = units::dB(std::max(totalAtt_.RefVal(), Mcl_c.RefVal()));
        power_.Att(applicablePl_);
        user->GetLteDev()->AddCellInfo(ae->ReadId(), cell_, power_, totalAtt_, ae->ReadConf().GetType());
    }
    END;
}

RxPower::RxPower(AntennaType_e ant, PropType_e prop)
: m_antennaType(ant)
, m_propType(prop)
{
    BEG END;
}

void
RxPower::operator()(gnsm::Ptr_t<User> user, gnsm::Ptr_t<LteEnb> ae)
{
    BEG;
    LteRxPower(user, ae, m_antennaType, m_propType);
    END;
}