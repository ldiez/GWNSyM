#ifndef UIFILTERS_H
#define	UIFILTERS_H

#include "FilterInterfaceConstraint.h"
namespace gnsm
{

template <class F>
struct FilterTypeRef;

template <typename CLS, typename T>
struct FilterTypeRef<bool (CLS::* )( T& ) const>
{
    typedef T type;
};

template <typename CLS, typename T>
struct FilterTypeRef<bool (CLS::* )( T& ) >
{
    typedef T type;
};

template < typename C >
struct GetFilterType
{
    typedef typename std::remove_cv<typename FilterTypeRef
    < decltype ( &C::operator() ) >::type>::type type;
};

template < typename E, typename F >
auto Filter_impl ( std::vector<std::shared_ptr<E>> const& ic, F const& c )
{
    std::vector<boost::any> v_;
    std::for_each(ic.begin(), ic.end(), [&v_, c]( auto& item )
    {
        if ( c(*item) )
        {
            v_.push_back(item);
        }
    });
    return v_;
};

template < typename E, typename F >
auto Filter ( std::vector<std::shared_ptr<E>> const& ic, F const& c )
{
    HAS_FILTER_INTERFACE(F, E);
    return Filter_impl(ic, c);
};

} // namespace gnsm
#endif	/* UIFILTERS_H */