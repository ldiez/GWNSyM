#ifndef UNWRAPPERUTILS_H
#define UNWRAPPERUTILS_H

#include "TypeUtils.h"

namespace gnsm
{
namespace ts
{

struct Pair_trait
{
};

struct Map_trait
{
};

struct Vec_trait
{
};

struct Ptr_trait
{
};

struct Ref_trait
{
};

template <class F>
struct ArgType;

template <typename CLS, typename... T>
struct ArgType<void (CLS::*)(T...) >
{
    typedef std::tuple<T...> type;
};

template < typename C, int I>
struct GetOrigType;

template < typename C >
struct GetOrigType<C, 0>
{
    typedef typename std::tuple_element < 0, typename ArgType
    < decltype(&C::operator())>::type>::type type;
};

template < typename C >
struct GetOrigType<C, 1>
{
    typedef typename std::tuple_element < 1, typename ArgType
    < decltype(&C::operator())>::type>::type type;
};

template < typename C >
struct GetOrigType<C, 2>
{
    typedef typename std::tuple_element < 2, typename ArgType
    < decltype(&C::operator())>::type>::type type;
};

template < typename T >
struct IsRef;

template < typename T >
struct IsRef<T&> : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsRef<T const&> : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsRef : std::false_type
{
    typedef EMPTY type;
};

template < typename T >
struct IsPointer;

template < typename T >
struct IsPointer<std::shared_ptr<T>> : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsPointer<std::shared_ptr<T>&> : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsPointer<std::shared_ptr<T> const&> : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsPointer : std::false_type
{
    typedef EMPTY type;
};

template < typename T >
struct IsVector;

template < typename T >
struct IsVector<std::vector<std::shared_ptr<T>> > : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsVector<std::vector<std::shared_ptr<T>>&> : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsVector<std::vector<std::shared_ptr<T>> const&> : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsVector : std::false_type
{
    typedef EMPTY type;
};

template < typename T >
struct IsMap;

template < typename T >
struct IsMap<std::unordered_multimap<std::uint32_t, std::shared_ptr<T>> > : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsMap<std::unordered_multimap<std::uint32_t, std::shared_ptr<T>> &> : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsMap<std::unordered_multimap<std::uint32_t, std::shared_ptr<T>> const&> : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsMap : std::false_type
{
    typedef EMPTY type;
};

template < typename T >
struct IsPair;

template < typename T >
struct IsPair<std::pair<const std::uint32_t, std::shared_ptr<T>> > : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsPair<std::pair<const std::uint32_t, std::shared_ptr<T>> &> : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsPair<std::pair<const std::uint32_t, std::shared_ptr<T>> const&> : std::true_type
{
    typedef T type;
};

template < typename T >
struct IsPair : std::false_type
{
    typedef EMPTY type;
};
/************************************************************************************************/

template < typename T, bool REF, bool PTR, bool VEC, bool MAP, bool PAIR >
struct GetRawType;

template < typename T >
struct GetRawType<T, true, false, false, false, false>
{
    typedef Ref_trait type;
    typedef typename IsRef<T>::type raw_type;
};

template < typename T >
struct GetRawType<T, false, true, false, false, false>
{
    typedef Ptr_trait type;
    typedef typename IsPointer<T>::type raw_type;
};

template < typename T >
struct GetRawType<T, false, false, true, false, false>
{
    typedef Vec_trait type;
    typedef typename IsVector<T>::type raw_type;
};

template < typename T >
struct GetRawType<T, false, false, false, true, false>
{
    typedef Map_trait type;
    typedef typename IsMap<T>::type raw_type;
};

template < typename T >
struct GetRawType<T, false, false, false, false, true>
{
    typedef Pair_trait type;
    typedef typename IsPair<T>::type raw_type;
};

template < typename T, int I >
struct GetType;

template < typename T >
struct GetType<T, 0>
{
private:
    typedef typename GetOrigType<T, 0>::type orig_type;
    using aux = GetRawType<orig_type,
            IsRef<orig_type>::value && !IsPointer<orig_type>::value && !IsVector<orig_type>::value
            && !IsMap<orig_type>::value && !IsPair<orig_type>::value, IsPointer<orig_type>::value,
            IsVector<orig_type>::value, IsMap<orig_type>::value, IsPair<orig_type>::value>;
public:
    typedef typename aux::type type_trait;
    typedef typename aux::raw_type raw_type;
};

template < typename T >
struct GetType<T, 1>
{
private:
    typedef typename GetOrigType<T, 1>::type orig_type;
    using aux = GetRawType<orig_type,
            IsRef<orig_type>::value && !IsPointer<orig_type>::value && !IsVector<orig_type>::value
            && !IsMap<orig_type>::value && !IsPair<orig_type>::value, IsPointer<orig_type>::value,
            IsVector<orig_type>::value, IsMap<orig_type>::value, IsPair<orig_type>::value>;
public:
    typedef typename aux::type type_trait;
    typedef typename aux::raw_type raw_type;
};

template < typename T >
struct GetType<T, 2>
{
private:
    typedef typename GetOrigType<T, 2>::type orig_type;
    using aux = GetRawType<orig_type,
            IsRef<orig_type>::value && !IsPointer<orig_type>::value && !IsVector<orig_type>::value
            && !IsMap<orig_type>::value && !IsPair<orig_type>::value, IsPointer<orig_type>::value,
            IsVector<orig_type>::value, IsMap<orig_type>::value, IsPair<orig_type>::value>;
public:
    typedef typename aux::type type_trait;
    typedef typename aux::raw_type raw_type;
};

template <class F>
struct ConfigType;

template <typename CLS, typename T>
struct ConfigType<void (CLS::*)(T&) >
{
    typedef T type;
};

template < typename T, typename Foo = void >
struct ByPassSetConfiguration;

template < typename T >
struct ByPassSetConfiguration<T, std::true_type>
{
    typedef typename std::remove_cv<typename
    ConfigType<decltype(&T::SetConfiguration)>::type>::type type;
};

template < typename T >
struct ByPassSetConfiguration<T, std::false_type>
{
    typedef EMPTY type;
};

template < typename T >
using DeducedConfigType_t = typename ByPassSetConfiguration<T,
typename has_just_function_SetConfiguration<T>::type>::type;

} // namespace ts
} // namespace gnsm

#endif /* UNWRAPPERUTILS_H */