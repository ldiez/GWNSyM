#include "System.h"
#include "Log.h"

namespace gnsm
{

LOG_REGISTER_MODULE("gnsm::System")

System&
System::AddGlobalConfigParam(GLOB_CONF const& i, NamePath_t&& path)
{
    BEG;
    if (i == GLOB_CONF::TOTAL_TIME)
    {
        m_globConfig.SetTotalTimePath(std::forward<NamePath_t>(path));
    }
    else
    {
        m_globConfig.SetSnapshotTimePath(std::forward<NamePath_t>(path));
    }
    END;
    return *this;
}

System&
System::AddGlobalConfigFinal(std::string&& finalName, NamePath_t&& path)
{
    BEG;
    m_globConfig.AddFinalType(std::forward<std::string>(finalName), std::forward<NamePath_t>(path));
    END;
    return *this;
}

System&
System::Deploy()
{
    BEG;
    auto items_ = m_globConfig.ReadFinalNamesList();
    for (auto& item_ : items_)
    {
        DBG("Creating types of ", item_);
        m_am.Create(item_, m_globConfig.NumberOf(item_));
    }
    END;
    return *this;
}

System&
System::Aggregate(std::string&& name, std::vector<std::string> const& strVec)
{
    BEG;
    m_am.Aggregate(std::forward<std::string>(name), strVec);
    END;
    return *this;
}

void
System::Run(void)
{
    BEG;
    for (auto& item_ : m_initializers)
    {
        (*item_) (0u);
    }
    m_initializers.clear(); 

    for (auto i = 1u; i <= m_totalSnapshots; ++i)
    {
        for (auto& item_ : m_actions)
        {
            (*item_)(i);
        }
    }

    for (auto& item_ : m_finalizers)
    {
        (*item_) (m_totalSnapshots);
    }
    m_finalizers.clear(); 
    m_actions.clear();
    END;
}

void
System::PrintTree(ts::NAME const& name) const
{
    BEG;
    m_am.PrintTree(name);
    END;
}

void
System::PrintInstances(ts::NAME const& name) const
{
    BEG;
    m_am.PrintInstances(name);
    END;
}

void
System::PrintAll(ts::NAME const& name) const
{
    BEG;
    m_am.PrintAll(name);
    END;
}

} // namespace gnsm
