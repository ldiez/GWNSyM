#include "EnbFixedLocator.h"
#include "Log.h"
#include "LteEnb.h"

LOG_REGISTER_MODULE("EnbFixedLocator")

EnbFixedLocator::EnbFixedLocator(std::vector<Position> pos) : m_pos(pos)
{
    BEGEND;
}

void
EnbFixedLocator::operator()(gnsm::Vec_t<LteEnb> es)
{
    BEG;
    MSG_ASSERT(es.size() <= m_pos.size(), "Not enough positions for eNB vector");
    for (auto i = 0u; i < es.size(); ++i)
    {
        es.at(i)->SetPosition(m_pos.at(i));
    }
    END;
}



