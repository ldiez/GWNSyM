#ifndef FILTERANDINTERFACEHELPER_H
#define	FILTERANDINTERFACEHELPER_H

#include "SystemUtils.h"
#include "filter-interface/Filter.h"
#include "filter-interface/Interface.h"

namespace gnsm
{
namespace detail
{

template < typename F > void
DoApplyFilter ( LocalItems_t& li, ts::AssetManager const& am, F const& f,
                std::string const& setName, std::string&& newName )
{
    auto type_ = FindTypes(am, setName);
    MSG_ASSERT(type_.size() > 0, "Not found any set with name ", setName);

    std::vector < std::shared_ptr < typename GetFilterType<F>::type > > vec0_;
    for ( auto& item_ : type_ )
    {
        auto casted_ = std::dynamic_pointer_cast < ts::Type< typename GetFilterType<F>::type,
                ts::DeducedConfigType_t< typename GetFilterType<F>::type > > >( item_ );
        MSG_ASSERT(casted_ != nullptr, "Bad type deduction, please check that the structure "
                   "passed can be called with references of selected types");
        auto aux_ = casted_->GetRawItems();
        vec0_.reserve(vec0_.size() + aux_.size());
        vec0_.insert(vec0_.end(), aux_.begin(), aux_.end());
    }

    li.emplace(std::forward<std::string>( newName ), Filter(vec0_, f));
}

template < typename I > void
DoApplyInterface ( LocalItems_t& li, ts::AssetManager const& am,
                   std::string const& setName, std::string&& newName )
{
    auto type_ = FindTypes(am, setName);
    MSG_ASSERT(type_.size() > 0, "Not found any set with name ", setName);

    std::vector < std::shared_ptr < typename GetInterfaceType<I>::type > > vec0_;
    for ( auto& item_ : type_ )
    {
        auto casted_ = std::dynamic_pointer_cast < ts::Type< typename GetInterfaceType<I>::type,
                ts::DeducedConfigType_t< typename GetInterfaceType<I>::type > > >( item_ );
        MSG_ASSERT(casted_ != nullptr, "Bad type deduction, please check that the structure "
                   "passed can be called with references of selected types");
        auto aux_ = casted_->GetRawItems();
        vec0_.reserve(vec0_.size() + aux_.size());
        vec0_.insert(vec0_.end(), aux_.begin(), aux_.end());
    }
    li.emplace(std::forward<std::string>( newName ), Wrap<I>( vec0_ ));
}

}
} // namespace gnsm

#endif	/* FILTERANDINTERFACEHELPER_H */

