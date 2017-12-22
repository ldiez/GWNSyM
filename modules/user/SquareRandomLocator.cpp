#include "SquareRandomLocator.h"
#include "Log.h"

LOG_REGISTER_MODULE("SquareRandomLocator");
static const double UserHeight = 1.5;

SquareRandomLocator::SquareRandomLocator(units::m side)
: m_re(std::random_device()())
, m_xDist(0, side.RawVal())
, m_yDist(0, side.RawVal())
{
    BEG;
    END;
}

SquareRandomLocator::SquareRandomLocator(units::m min, units::m max)
: m_re(std::random_device()())
, m_xDist(min.RawVal(), max.RawVal())
, m_yDist(min.RawVal(), max.RawVal())
{
    BEG;
    END;
}

SquareRandomLocator::SquareRandomLocator(units::m xmin, units::m xmax, units::m ymin, units::m ymax)
: m_re(std::random_device()())
, m_xDist(xmin.RawVal(), xmax.RawVal())
, m_yDist(ymin.RawVal(), ymax.RawVal())
{
    BEG;
    END;
}

void
SquareRandomLocator::operator()(gnsm::Ptr_t<User> user)
{
    BEG;
    auto x = m_xDist(m_re);
    auto y = m_yDist(m_re);
    user->SetPosition({x, y, UserHeight});
    DBG("User ", user->ReadId(), " at ", user->ReadPosition());
    END;
}

void
SquareRandomLocator::SetIteration(std::uint32_t iter)
{
    BEG;
    END;
}

