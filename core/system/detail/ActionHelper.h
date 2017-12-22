#ifndef ACTIONHELPER_H
#define	ACTIONHELPER_H

#include "GenericAction.h"
#include "type-system/UnwrapperUtils.h"

namespace gnsm
{
namespace detail
{

/**************************************************************************************************/
template < typename T >
void FillInVector ( std::vector < std::shared_ptr < T >>&vec, TypesVect_t const& type,
                    ItemsVector_t const& iv )
{

    for ( auto& item_ : type )
    {
        auto casted_ = std::dynamic_pointer_cast < ts::Type< T,
                ts::DeducedConfigType_t< T > > >( item_ );
        MSG_ASSERT(casted_ != nullptr, "Bad type deduction, please check that the structure "
                   "passed can be called with references of selected types");
        auto aux_ = casted_->GetRawItems();
        vec.reserve(vec.size() + aux_.size());
        vec.insert(vec.end(), aux_.begin(), aux_.end());
    }
    for ( auto& item_ : iv )
    {
        vec.push_back(boost::any_cast < std::shared_ptr < T >> ( item_ ));
    }
};

template < typename T > auto
GetMap ( TypesVect_t const& type )
{
    std::unordered_multimap<Id_t, std::shared_ptr < T>> map_;
    for ( auto& item_ : type )
    {
        auto casted_ = std::dynamic_pointer_cast < ts::Type< T,
                ts::DeducedConfigType_t< T > > >( item_ );
        MSG_ASSERT(casted_ != nullptr, "Bad type deduction, please check that the structure "
                   "passed can be called with references of selected types");
        auto aux_ = casted_->GetMapItems();
        map_.reserve(map_.size() + aux_.size());
        map_.insert(aux_.begin(), aux_.end());
    }
    return map_;
};

template < typename C, typename... ARGS > auto
DoCreateAction ( TypesVect_t const& type, ItemsVector_t&& iv, ARGS&&... args )
{
    using raw_type = typename ts::GetType<C, 0>::raw_type;

    std::vector < std::shared_ptr < raw_type > > vec0_;
    FillInVector<raw_type>( vec0_, type, iv );
    auto map_ = GetMap<raw_type> ( type );

    return std::unique_ptr<BaseAction>( std::make_unique < Action0
            < C, decltype(vec0_), decltype(map_), typename ts::GetType<C, 0>::type_trait,
            ARGS... > >
            ( std::move(vec0_), std::move(map_), std::forward<ARGS>( args )... ) );
}

template < typename C, typename... ARGS > auto
DoCreateAction ( TypesVect_t const& type0, TypesVect_t const& type1, ItemsVector_t&& iv0,
                 ItemsVector_t&& iv1, ARGS&&... args )
{
    using raw_type0 = typename ts::GetType<C, 0>::raw_type;
    std::vector < std::shared_ptr < raw_type0 > > vec0_;
    FillInVector<raw_type0>( vec0_, type0, iv0 );
    auto map0_ = GetMap<raw_type0> ( type0 );

    using raw_type1 = typename ts::GetType<C, 1>::raw_type;
    std::vector < std::shared_ptr < raw_type1 > > vec1_;
    FillInVector<raw_type1>( vec1_, type1, iv1 );
    auto map1_ = GetMap<raw_type1> ( type1 );
    
    return std::unique_ptr<BaseAction>( std::make_unique < Action1
            < C, decltype(vec0_), decltype(vec1_), decltype(map0_), decltype(map1_),
            typename ts::GetType<C, 0>::type_trait, typename ts::GetType<C, 1>::type_trait,
            ARGS...> >
            ( std::move(vec0_), std::move(vec1_), std::move(map0_), std::move(map1_),
            std::forward<ARGS>( args )... ) );
}

template < typename C, typename... ARGS > auto
DoCreateAction ( TypesVect_t const& type0, TypesVect_t const& type1, TypesVect_t const& type2,
                 ItemsVector_t&& iv0, ItemsVector_t&& iv1, ItemsVector_t&& iv2, ARGS&&... args )
{
    using raw_type0 = typename ts::GetType<C, 0>::raw_type;
    std::vector < std::shared_ptr < raw_type0 > > vec0_;
    FillInVector<raw_type0>( vec0_, type0, iv0 );
    auto map0_ = GetMap<raw_type0> ( type0 );

    using raw_type1 = typename ts::GetType<C, 1>::raw_type;
    std::vector < std::shared_ptr < raw_type1 > > vec1_;
    FillInVector<raw_type1>( vec1_, type1, iv1 );
    auto map1_ = GetMap<raw_type1> ( type1 );

    using raw_type2 = typename ts::GetType<C, 2>::raw_type;
    std::vector < std::shared_ptr < raw_type2 > > vec2_;
    FillInVector<raw_type2>( vec2_, type2, iv2 );
    auto map2_ = GetMap<raw_type2> ( type2 );

    return std::unique_ptr<BaseAction>( std::make_unique < Action2
            < C, decltype(vec0_), decltype(vec1_), decltype(vec2_), 
            decltype(map0_), decltype(map1_), decltype(map2_),
            typename ts::GetType<C, 0>::type_trait, typename ts::GetType<C, 1>::type_trait,
            typename ts::GetType<C, 2>::type_trait, ARGS...> >
            ( std::move(vec0_), std::move(vec1_), std::move(vec2_),
            std::move(map0_), std::move(map1_), std::move(map2_), std::forward<ARGS>( args )... ) );
}

template < typename C, typename... ARGS > auto
CreateAction ( ts::AssetManager const& am, LocalItems_t const& li, std::string const& name,
               ARGS&&... args )
{
    auto type_ = FindTypes(am, name);
    auto localItems_ = FindInLocal(li, name);
    return DoCreateAction<C>( type_, std::move(localItems_), std::forward<ARGS>( args )... );
}

template < typename C, typename... ARGS > auto
CreateAction ( ts::AssetManager const& am, LocalItems_t const& li, std::string const& name0,
               std::string const& name1, ARGS&&... args )
{
    auto type0_ = FindTypes(am, name0);
    auto type1_ = FindTypes(am, name1);
    auto localItems0_ = FindInLocal(li, name0);
    auto localItems1_ = FindInLocal(li, name1);
    return DoCreateAction<C>( type0_, type1_, std::move(localItems0_), std::move(localItems1_),
            std::forward<ARGS>( args )... );
}

template < typename C, typename... ARGS > auto
CreateAction ( ts::AssetManager const& am, LocalItems_t const& li, std::string const& name0,
               std::string const& name1, std::string const& name2, ARGS&&... args )
{
    auto type0_ = FindTypes(am, name0);
    auto type1_ = FindTypes(am, name1);
    auto type2_ = FindTypes(am, name2);
    auto localItems0_ = FindInLocal(li, name0);
    auto localItems1_ = FindInLocal(li, name1);
    auto localItems2_ = FindInLocal(li, name2);
    return DoCreateAction<C>( type0_, type1_, type2_, std::move(localItems0_),
            std::move(localItems1_), std::move(localItems2_), std::forward<ARGS>( args )... );
}

} // namespace detail
} // namespace gnsm

#endif	/* ACTIONHELPER_H */