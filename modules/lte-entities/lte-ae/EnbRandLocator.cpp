#include "EnbRandLocator.h"
#include "Log.h"
#include "LteEnb.h"

LOG_REGISTER_MODULE("EnbRandLocator")

static const double MacroHeight_s = 25.0;
static const double PicoHeight_s = 10.0;

EnbRandLocator::EnbRandLocator(units::m side)
: m_re(std::random_device()())
, m_xDist(0, side.RawVal())
, m_yDist(0, side.RawVal()) {
}

EnbRandLocator::EnbRandLocator(units::m min, units::m max)
: m_re(std::random_device()())
, m_xDist(min.RawVal(), max.RawVal())
, m_yDist(min.RawVal(), max.RawVal()) {
    BEG;
    END;
}

EnbRandLocator::EnbRandLocator(units::m xmin, units::m xmax, units::m ymin, units::m ymax)
: m_re(std::random_device()())
, m_xDist(xmin.RawVal(), xmax.RawVal())
, m_yDist(ymin.RawVal(), ymax.RawVal()) {
    BEG;
    END;
}

void
EnbRandLocator::operator()(gnsm::Ptr_t<LteEnb> e) {
    BEG;
    auto x = m_xDist(m_re);
    auto y = m_yDist(m_re);
    auto eight = e->GetConfiguration().GetType() == EnbType::MACRO ?
            MacroHeight_s : PicoHeight_s;
    e->SetPosition({x, y, eight});
    END;
}


