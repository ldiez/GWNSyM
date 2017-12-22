#ifndef GENERICACTION_H
#define	GENERICACTION_H

#include "type-system/AssetManager.h"
#include "SystemUtils.h"
#include "ActionUtils.h"

namespace gnsm
{
namespace detail
{

template < typename ACTION >
typename std::enable_if<has_function_weak_SetIteration<ACTION, void, 
        std::uint32_t >::value, void>::type
SetIteration (ACTION& act, std::uint32_t iter)
{
    act.SetIteration(iter);
}

template < typename ACTION >
typename std::enable_if<!has_function_weak_SetIteration<ACTION, void, 
        std::uint32_t >::value, void>::type
SetIteration (ACTION&, std::uint32_t)
{
}

struct BaseAction
{
    virtual ~BaseAction ( ) = default;

    void operator() ( std::uint32_t iter)
    {
        PerformAction( iter );
    }
private:
    virtual void PerformAction ( std::uint32_t ) = 0;
};

using ActionsVector_t = std::vector<std::unique_ptr<BaseAction>>;

/**
 * \description Simple wrapper to store an action and a reference to the items over which
 * the action will be applied
 */
template < typename C, typename ITEMS, typename ITEMSMAP, typename CHECK, typename... ARGS >
struct Action0 : BaseAction
{
    typedef C Action_t;

    Action0 ( ITEMS&& items, ITEMSMAP&& itemsmap, ARGS&&... args )
    : BaseAction ( )
    , m_action ( std::forward<ARGS>( args )... )
    , m_vect ( std::forward<ITEMS>( items ) )
    , m_map ( std::forward<ITEMSMAP>( itemsmap ) ) { }

private:

    void
    PerformAction ( std::uint32_t iter )
    {
        SetIteration<Action_t> (m_action, iter);
        Call1<Action_t, ITEMS, ITEMSMAP, CHECK> c_(m_action, m_vect, m_map);
    }

    Action_t m_action;
    ITEMS m_vect;
    ITEMSMAP m_map;
};

template < typename C, typename ITEMS0, typename ITEMS1, typename ITEMSMAP0, typename ITEMSMAP1,
typename CHECK0, typename CHECK1, typename... ARGS >
struct Action1 : BaseAction
{
    typedef C Action_t;

    Action1 ( ITEMS0&& items0, ITEMS1&& items1, ITEMSMAP0&& itemsmap0, ITEMSMAP1&& itemsmap1
              , ARGS&&... args )
    : BaseAction ( )
    , m_action ( std::forward<ARGS>(args)...)
    , m_vect0 ( std::forward<ITEMS0>( items0 ) )
    , m_vect1 ( std::forward<ITEMS1>( items1 ) )
    , m_map0 ( std::forward<ITEMSMAP0>( itemsmap0 ) )
    , m_map1 ( std::forward<ITEMSMAP1>( itemsmap1 ) ) { }

private:

    void PerformAction ( std::uint32_t iter )
    {
        SetIteration<Action_t> (m_action, iter);
        Call2<Action_t, ITEMS0, ITEMSMAP0, CHECK0, ITEMS1, ITEMSMAP1, CHECK1>
                c_(m_action, m_vect0, m_map0, m_vect1, m_map1);
    }

    Action_t m_action;
    ITEMS0 m_vect0;
    ITEMS1 m_vect1;
    ITEMSMAP0 m_map0;
    ITEMSMAP1 m_map1;
};

template < typename C, typename ITEMS0, typename ITEMS1, typename ITEMS2,
typename ITEMSMAP0, typename ITEMSMAP1, typename ITEMSMAP2,
typename CHECK0, typename CHECK1, typename CHECK2, typename... ARGS >
struct Action2 : BaseAction
{
    typedef C Action_t;

    Action2 ( ITEMS0&& items0, ITEMS1&& items1, ITEMS2&& items2,
              ITEMSMAP0&& itemsmap0, ITEMSMAP1&& itemsmap1, ITEMSMAP2&& itemsmap2, ARGS&&... args )
    : BaseAction ( )
    , m_action ( std::forward<ARGS>(args)...)
    , m_vect0 ( std::forward<ITEMS0>( items0 ) )
    , m_vect1 ( std::forward<ITEMS1>( items1 ) )
    , m_vect2 ( std::forward<ITEMS2>( items2 ) )
    , m_map0 ( std::forward<ITEMSMAP0>( itemsmap0 ) )
    , m_map1 ( std::forward<ITEMSMAP1>( itemsmap1 ) )
    , m_map2 ( std::forward<ITEMSMAP2>( itemsmap2 ) ) { }

private:

    void PerformAction ( std::uint32_t iter )
    {
        SetIteration<Action_t> (m_action, iter);
        Call3<Action_t, ITEMS0, ITEMSMAP0, CHECK0, ITEMS1, ITEMSMAP1, CHECK1,
                ITEMS2, ITEMSMAP2, CHECK2>
                c_(m_action, m_vect0, m_map0, m_vect1, m_map1, m_vect2, m_map2);
    }
    Action_t m_action;
    ITEMS0 m_vect0;
    ITEMS1 m_vect1;
    ITEMS2 m_vect2;
    ITEMSMAP0 m_map0;
    ITEMSMAP1 m_map1;
    ITEMSMAP2 m_map2;
};

} // namespace detail
} // namespace gnsm
#endif	/* GENERICACTION_H */