#include "UserFixedLocator.h"
#include "User.h"
#include "Log.h"


LOG_REGISTER_MODULE("UserFixedLocator")
//static const double UserHeight = 1.5;

UserFixedLocator::UserFixedLocator(std::vector<Position> pos) : m_pos(pos)
{
    BEGEND;
}

void
UserFixedLocator::operator()(gnsm::Vec_t<User> us)
{
    BEG;
    MSG_ASSERT(us.size() <= m_pos.size(), "Not enough positions for Users vector");
    for (auto i = 0u; i < us.size(); ++i)
    {
        us.at(i)->SetPosition(m_pos.at(i));
    }
    END;
}


