#ifndef ACTIONUTILS_H
#define ACTIONUTILS_H

#include "type-system/UnwrapperUtils.h"

namespace gnsm
{
namespace detail
{
/**************************************************************************************************/



template < typename ACTION, typename V, typename M, typename CHECK0, typename... OTHERS>
struct Call1;

template < typename ACTION, typename V, typename M, typename... ITEMS>
struct Call1<ACTION, V, M, ts::Ref_trait, ITEMS...>
{

    Call1(ACTION& act, V& v, M& m, ITEMS&... items)
    {
        for (auto& item_ : v)
        {
            act(items..., *item_);
        }
    }
};

template < typename ACTION, typename V, typename M, typename... ITEMS>
struct Call1<ACTION, V, M, ts::Ptr_trait, ITEMS...>
{

    Call1(ACTION& act, V& v, M& m, ITEMS&... items)
    {
        for (auto& item_ : v)
        {
            act(items..., item_);

        }
    }
};

template < typename ACTION, typename V, typename M, typename... ITEMS>
struct Call1<ACTION, V, M, ts::Vec_trait, ITEMS...>
{

    Call1(ACTION& act, V& v, M& m, ITEMS&... items)
    {
        act(items..., v);
    }
};

template < typename ACTION, typename V, typename M, typename... ITEMS>
struct Call1<ACTION, V, M, ts::Map_trait, ITEMS...>
{

    Call1(ACTION& act, V& v, M& m, ITEMS&... items)
    {
        act(items..., m);
    }
};

template < typename ACTION, typename V, typename M, typename... ITEMS>
struct Call1<ACTION, V, M, ts::Pair_trait, ITEMS...>
{

    Call1(ACTION& act, V& v, M& m, ITEMS&... items)
    {
        for (auto& item_ : m)
        {
            act(items..., item_);

        }
    }
};

/**************************************************************************************************/

template < typename ACTION, typename V0, typename M0, typename CHECK0,
typename V1, typename M1, typename CHECK1, typename... OTHERS>
struct Call2;

template < typename ACTION, typename V0, typename M0,
typename V1, typename M1, typename CHECK1, typename... ITEMS>
struct Call2<ACTION, V0, M0, ts::Ref_trait, V1, M1, CHECK1, ITEMS...>
{

    Call2(ACTION& act, V0& v0, M0& m0, V1& v1, M1& m1, ITEMS&... items)
    {
        for (auto& item_ : v0)
        {
            Call1<ACTION, V1, M1, CHECK1, ITEMS..., decltype(*item_) >
                    c_(act, v1, m1, items..., *item_);
        }
    }
};

template < typename ACTION, typename V0, typename M0,
typename V1, typename M1, typename CHECK1, typename... ITEMS>
struct Call2<ACTION, V0, M0, ts::Ptr_trait, V1, M1, CHECK1, ITEMS...>
{

    Call2(ACTION& act, V0& v0, M0& m0, V1& v1, M1& m1, ITEMS&... items)
    {
        for (auto& item_ : v0)
        {
            Call1<ACTION, V1, M1, CHECK1, ITEMS..., decltype(item_) >
                    c_(act, v1, m1, items..., item_);
        }
    }
};

template < typename ACTION, typename V0, typename M0,
typename V1, typename M1, typename CHECK1, typename... ITEMS>
struct Call2<ACTION, V0, M0, ts::Vec_trait, V1, M1, CHECK1, ITEMS...>
{

    Call2(ACTION& act, V0& v0, M0& m0, V1& v1, M1& m1, ITEMS&... items)
    {
        Call1<ACTION, V1, M1, CHECK1, ITEMS..., V0 > c_(act, v1, m1, items..., v0);
    }
};

template < typename ACTION, typename V0, typename M0,
typename V1, typename M1, typename CHECK1, typename... ITEMS>
struct Call2<ACTION, V0, M0, ts::Map_trait, V1, M1, CHECK1, ITEMS...>
{

    Call2(ACTION& act, V0& v0, M0& m0, V1& v1, M1& m1, ITEMS&... items)
    {
        Call1<ACTION, V1, M1, CHECK1, ITEMS..., M0 > c_(act, v1, m1, items..., m0);
    }
};

template < typename ACTION, typename V0, typename M0,
typename V1, typename M1, typename CHECK1, typename... ITEMS>
struct Call2<ACTION, V0, M0, ts::Pair_trait, V1, M1, CHECK1, ITEMS...>
{

    Call2(ACTION& act, V0& v, M0& m0, V1& v1, M1& m1, ITEMS&... items)
    {
        for (auto& item_ : m0)
        {
            Call1<ACTION, V1, M1, CHECK1, ITEMS..., decltype(item_) >
                    c_(act, v1, m1, items..., item_);
        }
    }
};

/**************************************************************************************************/

template < typename ACTION, typename V0, typename M0, typename V1, typename M1,
typename V2, typename M2, typename CHECK0, typename... OTHERS>
struct Call3;

template < typename ACTION, typename V0, typename M0, typename V1, typename M1,
typename CHECK1, typename V2, typename M2, typename CHECK2, typename... ITEMS>
struct Call3< ACTION, V0, M0, ts::Ref_trait, V1, M1, CHECK1, V2, M2, CHECK2, ITEMS...>
{

    Call3(ACTION& act, V0& v0, M0& m0, V1& v1, M1& m1, V2& v2, M2& m2, ITEMS&... items)
    {
        for (auto& item_ : v0)
        {
            Call2<ACTION, V1, M1, CHECK1, V2, M2, CHECK2, ITEMS..., decltype(*item_) >
                    c_(act, v1, m1, v2, m2, items..., *item_);
        }
    }
};

template < typename ACTION, typename V0, typename M0, typename V1, typename M1,
typename CHECK1, typename V2, typename M2, typename CHECK2, typename... ITEMS>
struct Call3< ACTION, V0, M0, ts::Ptr_trait, V1, M1, CHECK1, V2, M2, CHECK2, ITEMS...>
{

    Call3(ACTION& act, V0& v0, M0& m0, V1& v1, M1& m1, V2& v2, M2& m2, ITEMS&... items)
    {
        for (auto& item_ : v0)
        {
            Call2<ACTION, V1, M1, CHECK1, V2, M2, CHECK2, ITEMS..., decltype(item_) >
                    c_(act, v1, m1, v2, m2, items..., item_);
        }
    }
};

template < typename ACTION, typename V0, typename M0, typename V1, typename M1,
typename CHECK1, typename V2, typename M2, typename CHECK2, typename... ITEMS>
struct Call3< ACTION, V0, M0, ts::Vec_trait, V1, M1, CHECK1, V2, M2, CHECK2, ITEMS...>
{

    Call3(ACTION& act, V0& v0, M0& m0, V1& v1, M1& m1, V2& v2, M2& m2, ITEMS&... items)
    {
        Call2<ACTION, V1, M1, CHECK1, V2, M2, CHECK2, ITEMS..., V0 >
                c_(act, v1, m1, v2, m2, items..., v0);
    }
};

template < typename ACTION, typename V0, typename M0, typename V1, typename M1,
typename CHECK1, typename V2, typename M2, typename CHECK2, typename... ITEMS>
struct Call3< ACTION, V0, M0, ts::Map_trait, V1, M1, CHECK1, V2, M2, CHECK2, ITEMS...>
{

    Call3(ACTION& act, V0& v0, M0& m0, V1& v1, M1& m1, V2& v2, M2& m2, ITEMS&... items)
    {
        Call2<ACTION, V1, M1, CHECK1, V2, M2, CHECK2, ITEMS..., M0 >
                c_(act, v1, m1, v2, m2, items..., m0);
    }
};

template < typename ACTION, typename V0, typename M0, typename V1, typename M1,
typename CHECK1, typename V2, typename M2, typename CHECK2, typename... ITEMS>
struct Call3< ACTION, V0, M0, ts::Pair_trait, V1, M1, CHECK1, V2, M2, CHECK2, ITEMS...>
{

    Call3(ACTION& act, V0& v0, M0& m0, V1& v1, M1& m1, V2& v2, M2& m2, ITEMS&... items)
    {
        for (auto& item_ : m0)
        {
            Call2<ACTION, V1, M1, CHECK1, V2, M2, CHECK2, ITEMS..., decltype(item_) >
                    c_(act, v1, m2, v2, m2, items..., item_);
        }
    }
};

} // namespace detail
}// namespace gnsm



#endif /* ACTIONUTILS_H */