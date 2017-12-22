#include <algorithm>
#include <unistd.h>

#include "AssetManager.h"
#include "NameSearch.h"
#include "Log.h"

LOG_REGISTER_MODULE ( "ts::AssetManager" );

namespace gnsm
{
namespace ts
{

void
AssetManager::Create ( std::string const& name, std::uint32_t num )
{
    BEG;
    auto proto_ = FindPrototype(name);
    auto newType_ = DoDeploy(proto_);
    MSG_ASSERT (newType_ != nullptr, "Error in creating type");
    BackRegister(newType_->ReadName(), newType_);
    newType_->Create(num, newType_->ReadName());
}

void
AssetManager::BackRegister ( std::string str, BaseType::BaseTypePtr_t ptr )
{
    BEG;
    DBG("Quick search: adding ", str);
    auto check_ = m_quickMap.emplace(std::move(str), std::move(ptr));
    if (!check_.second)
    {
        MSG_ASSERT(false, "Error in quick search emplacement element " + str);
    }
    END;
}

AssetManager::Typesvect_t
AssetManager::Search ( std::vector<std::string> const& strVec ) const
{
    BEG;
    std::vector<BaseType::BaseTypePtr_t> ret_;
    for ( auto& name_ : strVec )
    {
        for ( auto& item_ : m_quickMap )
        {
            if ( CheckNames(item_.first, name_) )
            {
                ret_.push_back(item_.second);
            }
        }
    }
    return ret_;
}

AssetManager::Typesvect_t
AssetManager::SearchAggretated ( std::string const& name ) const
{
    BEG;
    std::vector<BaseType::BaseTypePtr_t> ret_;
    auto iter_ = m_aggregated.find(name);
    if (iter_ != m_aggregated.end() )
    {
        ret_ = iter_->second;
    }
    return ret_;
}


void 
AssetManager::Aggregate ( std::string&& newName, std::vector<std::string> const& strVec )
{
    std::vector<BaseType::BaseTypePtr_t> ret_;
    for ( auto& name_ : strVec )
    {
        for ( auto& item_ : m_quickMap )
        {
            if ( CheckNames(item_.first, name_) )
            {
                ret_.push_back(item_.second);
            }
        }
    }
    m_aggregated.emplace(std::forward<std::string>(newName), ret_);
}

void
AssetManager::PrintTree ( NAME const& name ) const
{
    BEG;
    std::cout << std::endl;
    std::cout << "================= CREATION TREE =================" << std::endl;
    for ( auto& item_ : m_typeMap )
    {
        item_.second->PrintInners("  ", name);
    }
    std::cout << "=================================================" << std::endl;
    std::cout << std::endl;
}

void
AssetManager::PrintInstances ( NAME const& name ) const
{
    BEG;
    std::cout << std::endl;
    std::cout << "================= INSTANCIATED TREE =================" << std::endl;
    for ( auto& item_ : m_typeMap )
    {
        item_.second->PrintInstancesCounter("  ", name);
    }
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
}

void
AssetManager::PrintAll ( NAME const& name ) const
{
    BEG;
    std::cout << std::endl;
    std::cout << "================= ASSET MANAGER CONTENT =================" << std::endl;
    for ( auto& item_ : m_typeMap )
    {
        item_.second->PrintAllContent("  ", name);
    }
    std::cout << "=========================================================" << std::endl;
    std::cout << std::endl;
}

AssetManager::PrototypesMap_t::const_iterator
AssetManager::FindPrototype ( std::string const& name ) const
{
    BEG;
    auto check_ = m_protoMap.find(name);
    MSG_ASSERT(check_ != m_protoMap.end(), "Looking for unregistered prototype ", name);
    END;
    return check_;
}

BaseType::BaseTypePtr_t
AssetManager::DoDeploy ( PrototypesMap_t::const_iterator const& iter )
{
    BEG;
    auto newType_ = iter->second->GetInstance();
    auto list_ = iter->second->GetInnerList();
    SpawnType(newType_, list_);
    Register(newType_);
    return newType_;
}

void
AssetManager::SpawnType ( BaseType::BaseTypePtr_t const& ptr, InnerItemsCounterList_t const& list )
{
    BEG;
    for ( auto& item : list )
    {
        KnotName kn_(ptr->ReadFullName());
        kn_.PushBack(item.first);
        MSG_ASSERT(!kn_.HasCycle(),
                "Cycle detected in the tree, check it!! Current branch is: ", std::string(kn_));

        auto iter_ = FindPrototype(item.first);
        auto innerList_ = iter_->second->GetInnerList();
        auto newType_ = iter_->second->GetInstance();
        ptr->AddInner(newType_, item.second);
        SpawnType(newType_, innerList_);
    }
}

void
AssetManager::Register ( BaseType::BaseTypePtr_t type )
{
    BEG;
    m_typeMap.emplace(type->ReadFullName(), std::move(type));
}

} // namespace ts
} // namespace gnsm