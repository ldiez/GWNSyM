#include "RxPower.h"
#include "PathLoss.h"
#include "Antenna.h"
#include "Log.h"
#include "Sinr.h"

LOG_REGISTER_MODULE("RxPower");
using namespace LTE;

// cable losses in dB
static const units::dB CouplingLoss_c(2.0); // cable
static const units::dB MinCL_c(70.0);
static const units::dB MaxCl_c(140.7);
static const Power MinRsRp_c(units::dBm(-140));
static const Power MaxRsRp_c(units::dBm(-44));

units::dB
ComputePathloss(EnbType type, gnsm::Ptr_t<User> user, gnsm::Ptr_t<LteEnb> ae, PropType_e prop) {
    BEG;
    auto hbs_ = units::m(ae->GetPosition().GetZ());
    auto hut_ = units::m(user->GetPosition().GetZ());
    auto frec_ = ae->GetConfiguration().GetFrequency();
    auto distance_ = GetPlanarDistance(user->GetPosition(), ae->GetPosition()).GetUnitsM();
    INFO("Distance ", distance_.RawVal());
    END;
    if (type == EnbType::MACRO) {
        INFO("Computing MACRO pathloss with distance ", distance_, " m");
        return (prop == PropType_e::FULL ?
                UmaPathloss(distance_, frec_, hbs_, hut_) :
                prop == PropType_e::LOS ?
                UmaPathloss_LOS(distance_, frec_, hbs_, hut_) :
                UmaPathloss_NLOS(distance_, frec_, hbs_, hut_));
    } else if (type == EnbType::MICRO) {
        INFO("Computing MICRO pathloss with distance ", distance_, "m");
        return (prop == PropType_e::FULL ?
                UmiPathloss(distance_, frec_, hbs_, hut_) :
                prop == PropType_e::LOS ?
                UmiPathloss_LOS(distance_, frec_, hbs_, hut_) :
                UmiPathloss_NLOS(distance_, frec_, hbs_, hut_));
    }
    INFO("Computing PICO pathloss with distance ", distance_, "m");
    return (prop == PropType_e::FULL ?
            UpiPathloss(distance_, frec_, hbs_, hut_) :
            prop == PropType_e::LOS ?
            UpiPathloss_LOS(distance_, frec_, hbs_, hut_) :
            UpiPathloss_NLOS(distance_, frec_, hbs_, hut_));
}

units::dB
ComputeAntenna(gnsm::Ptr_t<LteCell> cell, Position const& enbPos, Position const& uPos, AntennaType_e ant) {
    if (cell->GetConfiguration().GetType() != EnbType::MACRO) {
        return units::dB(0.0);
    }
    return (ant == AntennaType_e::HONLY ?
            ComputeAntennaAtt_Honly(cell->GetAzimut(), enbPos, uPos) :
            ComputeAntennaAtt(cell->GetAzimut(), enbPos, uPos));
}

void
LteRxPower(gnsm::Ptr_t<User> user, gnsm::Ptr_t<LteEnb> ae, AntennaType_e ant,
        PropType_e prop) {
    BEG;
    
    auto lteUe_ = user->GetLteDev();
    auto ueRxGain_ = lteUe_->GetConfiguration().GetRxGain();
    auto ueTxGain_ = lteUe_->GetConfiguration().GetTxGain();
    auto pl_ = ComputePathloss(ae->GetConfiguration().GetType(), user, ae, prop);
    for (auto& cell_ : ae->GetCells()) {
        auto cellTxGain_ = cell_->GetConfiguration().GetTxGain();
        auto cellRxGain_ = cell_->GetConfiguration().GetRxGain();
        auto antennaAtt_ = ComputeAntenna(cell_, ae->GetPosition(),
                user->GetPosition(), ant);
        auto rsrp_ = cell_->GetConfiguration().GetTxpowerPerRe();
        INFO("============ Tx power per RE ", rsrp_.GetDbm(), " dBm");
        INFO("PATH LOSS  ", pl_, " dB");
        INFO("Cell gain  ", cellTxGain_, " dB");
        INFO("UE gain  ", ueRxGain_, " dB");
        auto ulAtt_ = pl_ - ueTxGain_ - cellRxGain_ + CouplingLoss_c + antennaAtt_;
        auto totalAtt_ = pl_ + CouplingLoss_c + antennaAtt_ - cellTxGain_ - ueRxGain_;
        auto applicablePl_ = totalAtt_;// > MinCL_c ? totalAtt_ : MinCL_c;
        rsrp_.Att(applicablePl_);
        INFO ("Rsrp ", rsrp_.GetDbm(), " dBm")
        rsrp_ = rsrp_ > MaxRsRp_c ? MaxRsRp_c : rsrp_;
        if (rsrp_ < MinRsRp_c) {
            WARN("MAX CL for ", cell_);
            user->GetLteDev()->AddCellInfo(ae->GetId(), cell_,
                    Power(units::dBW(TooLow_s)),
                    units::dB(TooHigh_s),
                    ae->GetConfiguration().GetType());
        } else {
            user->GetLteDev()->AddCellInfo(ae->GetId(), cell_, rsrp_, 
                    ulAtt_, ae->GetConfiguration().GetType());
        }
    }
    END;
}

RxPower::RxPower(AntennaType_e ant, PropType_e prop)
: m_antennaType(ant)
, m_propType(prop) {
    BEGEND;
}

void
RxPower::operator()(gnsm::Ptr_t<User> user, gnsm::Ptr_t<LteEnb> ae) {
    BEG;
    WARN("User ", user->GetId(), " with ae ", ae->GetId())
    LteRxPower(user, ae, m_antennaType, m_propType);
    END;
}