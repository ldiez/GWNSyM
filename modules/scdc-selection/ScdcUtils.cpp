#include "ScdcUtils.h"
#include "Log.h"

LOG_REGISTER_MODULE("ScdcUtils");

LocalTranslator::LocalTranslator(double backgroundLoad, double interLoad, units::dB picoCre)
: m_sel(picoCre)
, m_interLoadFactor(interLoad)
, m_backLoadFactor(backgroundLoad)
{
}

TranslatedVec_t
LocalTranslator::Translate(gnsm::Vec_t<User> const& users, gnsm::Vec_t<LteCell> const& inter)
{
    m_inter = &inter;
    TranslatedVec_t tv_(users.size());

    auto it_ = 0u;
    for (auto& item_ : users)
    {
        TranslatedInfo tinfo_;
        tinfo_.m_self = item_;
        DoSetPowers(item_->GetLteDev()->GetOrderedCellsDl(), tinfo_);
        tv_.at(it_) = tinfo_;
        ++it_;
    }
    SetDemand(users, tv_);
    SetIntPow(tv_);
    return tv_;
}

void
LocalTranslator::DoSetPowers(LteUe::SensedValues_t const& sensed, TranslatedInfo& info)
{
    double intFloor_(0.0);
    LteUe::SensedValues_t candidates_;
    SensedMap_t sMap_;

    /**
     * - Interfering cells are discarding for selection and their RSRP added as interference
     * - The other cells are locally stored for the RA and for the selection
     */
    for (auto& item_ : sensed)
    {
        auto mw_ = item_.m_rsrp.GetMilliWatt();

        if (IsInterferingFloor(item_.m_cell))
        {
            intFloor_ += (LTE::InterFactor_s * m_interLoadFactor * mw_);
        }
        else
        {
            intFloor_ += (LTE::SelfInterFactor_s * m_backLoadFactor * mw_);
            candidates_.push_back(item_);
            sMap_.insert(SensedMap_t::value_type{item_.m_cell, mw_});
        }
    }
    auto sel_ = m_sel.Select(candidates_);
    info.m_sensed = sMap_;
    info.m_currCell = sel_.first;
    info.m_interFloor = intFloor_;
    info.m_pow = sel_.second.GetMilliWatt();
}

bool
LocalTranslator::IsInterferingFloor(gnsm::Ptr_t<LteCell>const& cell) const
{
    auto check_ = std::find(m_inter->begin(), m_inter->end(), cell);
    return (check_ != m_inter->end());
}

void
LocalTranslator::SetDemand(gnsm::Vec_t<User> const& users, TranslatedVec_t& info)
{
    BEG;
    auto it_ = 0u;
    for (auto& item_ : users)
    {
        auto conn_ = item_->GetConnectionManager();
        auto& temp_ = info.at(it_);
        temp_.m_demand = conn_->DlDemand().GetKbps();
        ++it_;
    }
    END;
}

void
LocalTranslator::SetIntPow(TranslatedVec_t& info)
{
    BEG;

    for (auto& item_ : info)
    {
        item_.m_interference.resize(info.size());
        auto pos_ = 0u;
        for (auto& it_ : info)
        {
            if (item_.m_currCell == it_.m_currCell)
            {
                item_.m_interference.at(pos_) = 0.0;
            }
            else
            {
                auto iter_ = item_.m_sensed.find(it_.m_currCell);
                if (iter_ == item_.m_sensed.end())
                {
                    item_.m_interference.at(pos_) = 0.0;
                }
                else
                {
                    // the interference is divided by the cell capacity
                    auto cap_ = iter_->first->GetConfiguration().GetCapacity();
                    item_.m_interference.at(pos_) =
                            LTE::SelfInterFactor_s * iter_->second / (cap_);
                }
            }
            ++pos_;
        }
    }
    END;
}