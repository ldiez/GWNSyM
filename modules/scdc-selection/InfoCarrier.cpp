#include <vector>

#include "InfoCarrier.h"
#include "Log.h"
#include "LteUtils.h"

LOG_REGISTER_MODULE("InfoCarrier");

#define LEN_ASSERT MSG_ASSERT(n < m_units.size(), "The index " + std::to_string(n) + "is not present");

InfoCarrier::InfoCarrier()
{
    BEG END;
}

InfoCarrier::~InfoCarrier()
{
    BEG;
    END;
}

void
InfoCarrier::AddUnit(double target)
{
    BEG;
    m_units.push_back({target, -target, 0.0, 0.0, 0.0});
    END;
}

std::uint32_t
InfoCarrier::GetNumUnits(void) const
{
    BEG END;
    return m_units.size();
}

double
InfoCarrier::GetAssignment(std::uint32_t n) const
{
    BEG;
    LEN_ASSERT
    END;
    return m_units.at(n).m_current;
}

double
InfoCarrier::GetSeff(std::uint32_t n) const
{
    BEG;
    LEN_ASSERT
    END;
    return m_units.at(n).m_seff;
}

double
InfoCarrier::GetTarget(std::uint32_t n) const
{
    BEG;
    LEN_ASSERT
    END;
    return m_units.at(n).m_targetThput;
}

double
InfoCarrier::GetThputExcess(std::uint32_t n) const
{
    BEG;
    LEN_ASSERT
    END;
    return m_units.at(n).m_thputExcess;
}

double 
InfoCarrier::GetMutualInter(std::uint32_t n) const
{
    BEG;
    LEN_ASSERT
    END;
    return m_units.at(n).m_mutualInter;
}

void
InfoCarrier::SetAssignment(std::uint32_t n, double curr)
{
    BEG;
    LEN_ASSERT
    m_units.at(n).m_current = curr;
    END;
}

void
InfoCarrier::SetSeff(std::uint32_t n, double seff)
{
    BEG;
    LEN_ASSERT
    m_units.at(n).m_seff = seff;
    END;
}

void
InfoCarrier::SetThputExcess(std::uint32_t n, double excess)
{
    BEG;
    LEN_ASSERT
    m_units.at(n).m_thputExcess = excess;
    END;
}

void 
InfoCarrier::SetMutualInter(std::uint32_t n, double inter)
{
    BEG;
    LEN_ASSERT
    m_units.at(n).m_mutualInter = inter;
    END;
}

void 
InfoCarrier::Clear( void )
{
    BEG;
    m_units.clear();
    END;
}


const double InfoCarrier::ResFactor = LTE::BwEff_s * LTE::RbBw_s.RawVal();