#ifndef SYSTEMUTILS_H
#define	SYSTEMUTILS_H

#include "type-system/AssetManager.h"
#include <boost/any.hpp>

namespace gnsm
{
namespace detail
{
using ItemsVector_t = std::vector<boost::any>;
using TypesVect_t = ts::AssetManager::Typesvect_t;
using LocalItems_t = std::unordered_map<std::string, ItemsVector_t>;
using PersistentFilters_t = std::unordered_map<std::string, std::string>;

inline TypesVect_t
FindTypes ( ts::AssetManager const& am, std::string const& name )
{
    auto v_ = am.Search({ name });
    if ( v_.size() == 0 )
    {
        v_ = am.SearchAggretated(name);
    }

    return v_;
}

inline
ItemsVector_t
FindInLocal ( LocalItems_t const& li, std::string const& name )
{
    ItemsVector_t v_;

    auto iter_ = li.find(name);
    if ( iter_ != li.end() )
    {
        v_.reserve(v_.size() + iter_->second.size());
        v_.insert(v_.end(), iter_->second.begin(), iter_->second.end());
    }

    return v_;
}


} // namespace
} // namesapce gnsm

#endif	/* SYSTEMUTILS_H */

