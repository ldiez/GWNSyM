#include "User.h"
#include "Log.h"

LOG_REGISTER_MODULE("User");

User::User(gnsm::Id_t id) : m_id(id), m_pos(0.0, 0.0, 1.5), m_conf(nullptr)
{
    BEG END;
}

User::~User()
{
    BEG END;
}

void
User::SetConfiguration(UserConf const& conf)
{
    BEG;
    m_conf = &conf;
    END;
}

void
User::Aggregate(std::string const& name, gnsm::ts::Wrapper_t o)
{
    BEG;
    if (name == "LTE_UE")
    {
        o.Unwrap(m_lteUe);
        m_connMan.RegisterDevice(m_lteUe);
    }
    else if (name == "GENERIC_SERVICE" || name == "VIDEO" || name == "INTERNET" 
            || name == "VOICE")
    {
        gnsm::Ptr_t<Service> auxServ_;
        o.Unwrap(auxServ_);
        m_connMan.RegisterService(auxServ_);
    }

    END;
}

void
User::SetPosition(Position pos)
{
    BEG END;
    m_pos = pos;
}

Position const&
User::GetPosition(void) const
{
    BEG END;
    return m_pos;
}

gnsm::Id_t
User::GetId(void) const
{
    BEG END;
    return m_id;
}

gnsm::Ptr_t<LteUe>
User::GetLteDev(void)
{
    BEG END;
    return m_lteUe;
}

ConnectionManager* const 
User::GetConnectionManager ( void )
{
    BEG END;
    return &m_connMan;
}

UserConf const&
User::GetConfiguration(void) const
{
    BEG END;
    return *m_conf;
}

void 
User::CallUp ( void )
{
    BEG;
    m_lteUe->CallUp();
    m_connMan.CallUp();
    END;
}

bool 
User::IsActive ( void )
{
    BEG END;
    return (m_connMan.DlDemand().GetBps() > 0.0 || 
            m_connMan.UlDemand().GetBps() > 0.0);
}