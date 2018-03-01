#include <iomanip>
#include <complex>

#include "LteClusterLocatorSquare.h"
#include "CoreUtils.h"


#include <fstream>
#include <iostream>
#include <string>
#include <bits/random.h>
#include <cstdlib>

LOG_REGISTER_MODULE("LteClusterLocatorSquare");

static const double MacroHeight_s = 25.0;
static const double PicoHeight_s = 10.0;

LteClusterLocatorSquare::LteClusterLocatorSquare(units::m side, units::m isd, std::uint32_t rings,
        units::m maxRange, units::m minRange)
: m_side(side)
, m_isd(isd)
, m_rings(rings)
, m_maxRange(maxRange)
, m_minRange(minRange)
, m_distSide(m_minRange.RawVal(), m_maxRange.RawVal())
, m_distSide2(0, m_maxRange.RawVal())
, m_re(0)
, m_ctr(0u) {
    BEG;
    //    WARN("Side between ", m_distSide.min(), " and ", m_distSide.max())
    //    exit(-1);
    HexPosition();
    END;
}

void
LteClusterLocatorSquare::operator()(gnsm::Vec_t<LteCluster> cl) {
    BEG;
    for (auto & item_ : cl) {
        DeployMacro(item_);
        DeployPicos(item_);
    }
    END;
}

void
LteClusterLocatorSquare::DeployMacro(gnsm::Ptr_t<LteCluster> cl) {
    BEG;
    m_ctr = 0;
    auto coord_ = m_coord.at(m_ctr);
    auto macro_ = cl->GetMacro();
    //Para Scdc1
    macro_->SetPosition({coord_.first + m_side.RawVal() / 2.0,
        coord_.second + m_side.RawVal() / 2.0,
        MacroHeight_s});
    
    ++m_ctr;
    END;
}

void
LteClusterLocatorSquare::DeployPicos(gnsm::Ptr_t<LteCluster> ae) {
    BEG;
    auto picos_ = ae->GetPicos();
    auto macroPos_ = ae->GetMacro()->GetPosition();
    INFO("Macro at = ", macroPos_)

    INFO("Number of picos = ", picos_.size())
    for (auto i = 0u; i < picos_.size(); ++i) {

        auto ok = false;
        auto ctr = 0;
        while (!ok) {
            auto aux = GetRandomSide(macroPos_, i);
            auto toAdd = true;
            for (auto j = 0u; j < i; ++j) {
                if (GetPlanarDistance(picos_[j]->GetPosition(), aux).GetM() < 5) {
                    toAdd = false;
                }
            }
            if (toAdd) {
                picos_[i]->SetPosition(aux);
                INFO("Pico ", picos_[i]->GetId(), " at ", picos_[i] ->GetPosition());
                ok = true;
            }
            ++ctr;
            if (ctr == 10000) {
                ERROR("Too dense!!!!!!!!!!!!!!!!");
                exit(-1);
            }
        }
    }
    END;
}

Position
LteClusterLocatorSquare::GetRandomSide(Position center, int i) {
    BEG;
    auto x_ = 0.0;
    auto y_ = 0.0;
    if (i % 2 == 0) {
        x_ = m_distSide(m_re);
        y_ = m_distSide2(m_re);
    } else {
        x_ = m_distSide2(m_re);
        y_ = m_distSide(m_re);
    }

    x_ = (float(rand()) / RAND_MAX > 0.5 ? x_ : -x_);
    y_ = (float(rand()) / RAND_MAX > 0.5 ? y_ : -y_);
    END;
    return {center.GetX() + x_,
        center.GetY() + y_,
        PicoHeight_s};
}

void
LteClusterLocatorSquare::HexPosition(void) {
    BEG;
    auto d_ = m_isd.RawVal();
    auto ctr_ = 0u;

    for (auto i_ = 0u; i_ <= (m_rings - 1); ++i_) {
        auto y_ = (std::sqrt(3.0) * i_ * d_) / 2.0;
        for (auto j_ = 0u; j_ < (2 * m_rings - 1 - i_); ++j_) {
            auto x_ = (-double(2 * m_rings - i_ - 2) * d_) / 2.0 + j_*d_;
            ++ctr_;
            if (ctr_ == m_rings) {

                m_coord.insert(m_coord.begin(),{
                    x_, y_
                });
            } else {
                m_coord.push_back({x_, y_});
            }


            if (y_ != 0) {
                ++ctr_;
                if (ctr_ == m_rings) {

                    m_coord.insert(m_coord.begin(),{
                        x_, y_
                    });
                } else {
                    m_coord.push_back({x_, -y_});
                }

            }
        }
    }
    END;
}