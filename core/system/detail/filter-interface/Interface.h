#ifndef UIINTERFACE_H
#define	UIINTERFACE_H

#include "FilterInterfaceConstraint.h"

namespace gnsm
{

template <class F>
struct InterfaceType;

template <typename CLS, typename T>
struct InterfaceType<void (CLS::* )( std::shared_ptr<T> )>
{
    typedef T type;
};

template <typename CLS, typename T>
struct InterfaceType<void (CLS::* )( std::shared_ptr<T>& )>
{
    typedef T type;
};

template <typename CLS, typename T>
struct InterfaceType<void (CLS::* )( std::shared_ptr<T> const&) >
{
    typedef T type;
};

template < typename C >
struct GetInterfaceType
{
    typedef typename InterfaceType < decltype ( &C::Wrap ) >::type type;
};

template < typename I, typename V > auto
Wrap_impl ( std::vector<std::shared_ptr<V>> const& v )
{
    std::vector<boost::any> m_;

    std::for_each(v.begin(), v.end(), [&m_]( auto& item )
    {
        auto it_ = std::make_shared<I>( );
                  it_->Wrap(item);
                  m_.push_back(it_);
    });
    return m_;
};

template < typename I, typename V > auto
Wrap ( std::vector<std::shared_ptr<V>> const& m )
{
    
    return Wrap_impl<I>( m );
};


} // namespace gnsm
#endif	/* UIINTERFACE_H */

