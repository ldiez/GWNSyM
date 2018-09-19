#include "EnbHexLocator.h"
#include "Log.h"
#include "LteEnb.h"

LOG_REGISTER_MODULE("EnbHexLocator")

static const double MacroHeight_s = 25.0;
static const double PicoHeight_s = 10.0;

EnbHexLocator::EnbHexLocator(units::m side, units::m isd, std::uint32_t rings)
: m_side(side)
, m_isd(isd)
, m_rings(rings)
, m_ctr(0u) {
    BEG;
    HexPosition();
    END;
}

void
EnbHexLocator::operator()(gnsm::Vec_t<LteEnb> es) {
    BEG;
    for (auto& e : es) {
        auto coord_ = m_coord.at(m_ctr);
        e->SetPosition({coord_.first + m_side.RawVal() / 2.0,
            coord_.second + m_side.RawVal() / 2.0,
            MacroHeight_s});
        ++m_ctr;
    }
    END;
}

void
EnbHexLocator::HexPosition(void) {
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

