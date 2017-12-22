#include "GlobalConfig.h"
#include "Log.h"

namespace gnsm
{

LOG_REGISTER_MODULE ( "gnsm::GlobalConfig" );

void
GlobalConfig::AddFinalType ( std::string&& name, NamePath_t&& path )
{
    BEG;
    std::string name_ = std::forward<std::string>( name );
    auto check_ = m_finalPaths.insert(FinalPathMap_t::value_type{
        name_,
        std::forward<NamePath_t>( path )
    });

    if ( !check_.second )
    {
        check_.first->second = path;
    }
    else
    {
        m_finalNamesList.push_back(name_);
    }
}

void
GlobalConfig::SetNumberOf ( std::string const& name, std::uint32_t n )
{
    BEG;
    auto check_ = m_finalNumber.insert(FinalNumMap_t::value_type{
        name,
        n
    });
    
    if ( !check_.second )
    {
        check_.first->second = n;
    }
}

std::uint32_t
GlobalConfig::NumberOf ( std::string const& name )
{
    BEG;
    auto iter_ = m_finalNumber.find(name);
    return (iter_ == m_finalNumber.end() ? 0 : iter_->second );
}

void
GlobalConfig::SetTotalTimePath ( NamePath_t&& path )
{
    BEG;
    m_totalTime = std::forward<NamePath_t>(path);
}

void
GlobalConfig::SetSnapshotTimePath ( NamePath_t&& path )
{
    BEG;
    m_snapshotTime = std::forward<NamePath_t>(path);
}

NamePath_t 
GlobalConfig::GetTotalTimePath ( void )
{
    BEG;
    return m_totalTime;
}
    
NamePath_t const&
GlobalConfig::ReadSnapshotTimePath ( void ) const
{
    BEG;
    return m_snapshotTime;
}

std::list<std::string> const&
GlobalConfig::ReadFinalNamesList ( void ) const
{
    BEG;
    return m_finalNamesList;
}

NamePath_t const& 
GlobalConfig::ReadFinalPath ( std::string const& name) const
{
    BEG;
    auto check_ = m_finalPaths.find(name);
    
    if (check_ != m_finalPaths.end())
    {
        return check_->second;
    }
    else
    {
        MSG_ASSERT (false, "Not registered path for specified name");
        return INVALID_PATH;
    }
}

} // namespace gnsm