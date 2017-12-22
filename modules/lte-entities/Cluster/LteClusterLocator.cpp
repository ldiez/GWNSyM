#include <iomanip>
#include <complex>

#include "LteClusterLocator.h"
#include "Utils.h"

LOG_REGISTER_MODULE("LteClusterLocator");

static const double MacroHeight_s = 25.0;
static const double PicoHeight_s = 10.0;

LteClusterLocator::LteClusterLocator(units::m side, units::m isd, std::uint32_t rings,
                                     units::m maxRange, units::m minRange)
: m_side(side)
, m_isd(isd)
, m_rings(rings)
, m_maxRange(maxRange)
, m_minRange(minRange)
, m_distRange(m_minRange.RawVal(), m_maxRange.RawVal())
, m_distAngle(0.0, 360.0)
, m_re(std::random_device()())
, m_ctr(0u)
{
    BEG;
    HexPosition();
    END;
}

void LteClusterLocator::operator()(gnsm::Vec_t<LteCluster> cl)
{
    BEG;
    for (auto & item_ : cl)
    {
        DeployMacro(item_);
        DeployPicos(item_);
    }
    END;
}

void
LteClusterLocator::DeployMacro(gnsm::Ptr_t<LteCluster> cl)
{
    BEG;

    auto coord_ = m_coord.at(m_ctr);
    auto macro_ = cl->GetMacro();
    macro_->SetPosition({coord_.first + m_side.RawVal() / 2.0,
                        coord_.second + m_side.RawVal() / 2.0,
                        MacroHeight_s});
    ++m_ctr;
    END;
}

void
LteClusterLocator::DeployPicos(gnsm::Ptr_t<LteCluster> ae)
{
    BEG;
    auto picos_ = ae->GetPicos();
    auto macroPos_ = ae->GetMacro()->ReadPosition();


    for (auto& item_ : picos_)
    {

        item_->SetPosition(GetRandomAnnulus(macroPos_));
    }

    END;
}

Position
LteClusterLocator::GetRandomAnnulus(Position center)
{
    BEG;

    auto range_ = m_distRange(m_re);
    auto angle_ = m_distAngle(m_re);

    auto x_ = range_ * std::cos(angle_);
    auto y_ = range_ * std::sin(angle_);

    END;
    return
    {
        center.GetX() + x_, center.GetY() + y_, PicoHeight_s
    };
}

void
LteClusterLocator::HexPosition(void)
{
    BEG;
    auto d_ = m_isd.RawVal();
    auto ctr_ = 0u;

    for (auto i_ = 0u; i_ <= (m_rings - 1); ++i_)
    {
        auto y_ = (std::sqrt(3.0) * i_ * d_) / 2.0;
        for (auto j_ = 0u; j_ < (2 * m_rings - 1 - i_); ++j_)
        {
            auto x_ = (-double(2 * m_rings - i_ - 2) * d_) / 2.0 + j_*d_;
            ++ctr_;
            if (ctr_ == m_rings)
            {

                m_coord.insert(m_coord.begin(),
                {
                               x_, y_
                });
            }
            else
            {
                m_coord.push_back({x_, y_});
            }


            if (y_ != 0)
            {
                ++ctr_;
                if (ctr_ == m_rings)
                {

                    m_coord.insert(m_coord.begin(),
                    {
                                   x_, y_
                    });
                }
                else
                {
                    m_coord.push_back({x_, -y_});
                }

            }
        }
    }
    END;
}