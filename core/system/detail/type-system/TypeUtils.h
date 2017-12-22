#ifndef TYPEUTILS_H
#define	TYPEUTILS_H

#include "CoreUtils.h"
#include "InterfaceConstraints.h"

namespace gnsm
{
namespace ts
{

// \TODO it should be called type sysytem helpers
// \NOTE If you're reading this, I hope it is for fun!!

// handy empty class to discriminate 
struct EMPTY
{   
    EMPTY (  ) = default;
    EMPTY ( std::string const& ){}
};

/**
 * \description Generic configuration loader. If the type is empty it does nothing
 */
template < typename T >
inline void GenericLoadConfig ( std::shared_ptr<T> const& conf )
{
    conf->Configure();
}

template <>
inline void GenericLoadConfig<EMPTY> ( std::shared_ptr<EMPTY> const&) { }

/**
 * \description Generic internal list getter. If the type is empty it returns an empty list
 */
template < typename T, typename Foo = void >
struct GenericGetInnerList_if_possible;

template < typename T >
struct GenericGetInnerList_if_possible<T, std::true_type>
{

    InnerItemsCounterList_t operator() ( std::shared_ptr<T> const& conf )
    {
        return conf->ReadInnerConf();
    }
};

template < typename T >
struct GenericGetInnerList_if_possible<T, std::false_type>
{

    InnerItemsCounterList_t operator() ( std::shared_ptr<T> const& conf )
    {
        return
        {
        };
    }
};

template < typename T >
inline InnerItemsCounterList_t GenericGetInnerList ( std::shared_ptr<T> const& conf )
{
    return GenericGetInnerList_if_possible<T,
            typename has_function_weak_ReadInnerConf<T, InnerItemsCounterList_t>::type > ( )( conf );
}

template < >
inline InnerItemsCounterList_t GenericGetInnerList<EMPTY> ( std::shared_ptr<EMPTY> const&)
{
    return
    {
    };
}

/**
 * \description Generic configuration binder. If the configuration is empty it does nothing
 */
template < typename T, typename CONF, typename Foo = void>
struct GenericBindConfig_if_possible;

template < typename T, typename CONF>
struct GenericBindConfig_if_possible<T, CONF, std::true_type>
{

    void operator() ( std::shared_ptr<T> type, CONF const& conf )
    {
        type->SetConfiguration(conf);
    }
};

template < typename T, typename CONF>
struct GenericBindConfig_if_possible<T, CONF, std::false_type>
{

    void operator() ( std::shared_ptr<T> type, CONF const& conf ) { }
};

template < typename T, typename CONF>
struct GenericBindConfig
{

    void operator() ( std::shared_ptr<T> type, CONF const& conf )
    {
        GenericBindConfig_if_possible<T, CONF,
                typename has_function_weak_SetConfiguration<T, void, CONF & >::type > ( )( type, conf );
    }
};

template < typename T>
struct GenericBindConfig<T, EMPTY>
{

    void operator() ( std::shared_ptr<T> type, EMPTY const& conf ) { }
};

/**
 * \description Items creation: if the item can is constructible with in Id, it is constructed so, 
 * otherwise default constructor is called
 */

template < typename T >
using ByIdConstructible_t = typename std::is_constructible<T, Id_t>::type;
        
template < typename T, typename Foo = void>
struct GenericItemCreator;

template < typename T>
struct GenericItemCreator<T, std::true_type>
{
    template < typename C >
    void operator() (C& cont, Id_t id)
    {
        cont.emplace(id, std::make_shared<T>(id));
    }
};

template < typename T>
struct GenericItemCreator<T, std::false_type>
{
    template < typename C >
    void operator() ( C& cont, Id_t id)
    {
        cont.emplace(id, std::make_shared<T>());
    }
};


} // namespace ts
} // namespace gnsm
#endif	/* TYPEUTILS_H */