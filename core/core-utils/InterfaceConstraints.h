#ifndef INTERFACECONSTRINTS_H
#define	INTERFACECONSTRINTS_H

#include <type_traits>

namespace gnsm
{

#define PREAMBLE "CONSTRAINT FAILED IN " __FILE__ " : \n"

/**
 * \brief Check if a class has a function with a given name, can be called with a given 
 * set of parameters (it checks the types of those parameters) and returns a given type
 * Hence, the return type constraint is hard
 */
#define FUNCTION_MEMBER_HARD(NAME)                                                  \
template < typename TYPE, typename RETURN, typename... ARGS>                        \
struct has_function_hard_##NAME                                                     \
{                                                                                   \
    template < typename T >                                                         \
    static constexpr auto                                                           \
    Check ( T* ) -> typename                                                        \
        std::is_same< decltype ( std::declval<T>( ).NAME                            \
        ( std::declval<ARGS>( )... ) ), RETURN>::type;                              \
                                                                                    \
template<typename>                                                                  \
static constexpr std::false_type                                                    \
Check ( ... );                                                                      \
                                                                                    \
typedef decltype ( Check<TYPE>( 0 ) ) type;                                         \
                                                                                    \
static constexpr bool value = type::value;                                          \
}; 

/**
 * \brief Check if a class has a function with a given name, can be called with a given 
 * set of parameters (it checks the types of those parameters) and returns a type convertible to
 * another given type
 * Hence, the return type constraint is weak
 */
#define FUNCTION_MEMBER_WEAK(NAME)                                                  \
template < typename TYPE, typename RETURN, typename... ARGS>                        \
struct has_function_weak_##NAME                                                     \
{                                                                                   \
    template < typename T >                                                         \
    static constexpr auto                                                           \
    Check ( T* ) -> typename                                                        \
        std::is_convertible< decltype ( std::declval<T>( ).NAME                     \
        ( std::declval<ARGS>( )... ) ), RETURN>::type;                              \
                                                                                    \
template<typename>                                                                  \
static constexpr std::false_type                                                    \
Check ( ... );                                                                      \
                                                                                    \
typedef decltype ( Check<TYPE>( 0 ) ) type;                                         \
                                                                                    \
static constexpr bool value = type::value;                                          \
}; 

/**
 * \brief Check if a class has a function with a given name, just the name. That's all!!
 */
#define JUST_FUNCTION_MEMBER_NAME(NAME)                                             \
template < typename TYPE >                                                          \
struct has_just_function_##NAME                                                     \
{                                                                                   \
    template < typename T >                                                         \
    static constexpr std::true_type                                                 \
    Check ( decltype ( &T::NAME ) );                                                \
                                                                                    \
    template<typename>                                                              \
    static constexpr std::false_type                                                \
    Check ( ... );                                                                  \
                                                                                    \
    typedef decltype ( Check<TYPE>( 0 ) ) type;                                     \
                                                                                    \
    static constexpr bool value = type::value;                                      \
};

/**
 * \brief Check if a class has a given operator, can be called with a given 
 * set of parameters (it checks the types of those parameters) and returns a given type
 * Hence, the return type constraint is hard
 */
template < typename TYPE, typename RETURN, typename... ARGS>
struct is_callable_hard;

template < typename TYPE, typename RETURN, typename... ARGS>
struct is_callable_hard <TYPE, RETURN ( ARGS... )>
{
    template < typename T >
    static constexpr auto
     Check ( T* ) -> typename
    std::is_same< decltype ( std::declval<T>( ).operator() 
    ( std::declval<ARGS>( )... ) ) , RETURN>::type;

    template<typename>
    static constexpr std::false_type 
    Check ( ... );

    typedef decltype ( Check<TYPE>( 0 ) ) type;

    static constexpr bool value = type::value;
};

/**
 * \brief Check if a class has a given operator, can be called with a given 
 * set of parameters (it checks the types of those parameters) and the return type is convertible to another one
 * Hence, the return type constraint is weak
 */
template < typename TYPE, typename RETURN, typename... ARGS>
struct is_callable_weak;

template < typename TYPE, typename RETURN, typename... ARGS>
struct is_callable_weak<TYPE, RETURN ( ARGS... )>
{
    template < typename T >
    static constexpr auto
     Check ( T* ) -> typename
    std::is_convertible< decltype ( std::declval<T>( ).operator() ( std::declval<ARGS>( )... ) ) , RETURN>::type;

    template<typename>
    static constexpr std::false_type 
    Check ( ... );

    typedef decltype ( Check<TYPE>( 0 ) ) type;

    static constexpr bool value = type::value;
};

/**
 * \brief Check is callable regardless the parameters passed or returned
 */
template < typename TYPE >           
struct is_just_callable             
{                                    
    template < typename T >          
    static constexpr std::true_type  
    Check ( decltype ( &T::operator() ) );
    template<typename>                    
    static constexpr std::false_type      
    Check ( ... );                        
    typedef decltype ( Check<TYPE>( 0 ) ) type;                                     
    static constexpr bool value = type::value;                                      
};

/**
 * \brief Check if a class has a member with a given name and \convertible to other given type
 * Hence, the type restriction is weak
 */
#define VAR_MEMBER_WEAK(NAME)                                                   \
template < typename TYPE, typename MEMBER >                                     \
struct has_member_weak_##NAME                                                   \
{                                                                               \
    template < typename T >                                                     \
    static constexpr auto                                                       \
     Check ( T* ) -> typename                                                   \
    std::is_convertible< decltype ( std::declval<T>( ).NAME), MEMBER>::type;    \
                                                                                \
    template<typename>                                                          \
    static constexpr std::false_type                                            \
    Check ( ... );                                                              \
                                                                                \
    typedef decltype ( Check<TYPE>( 0 ) ) type;                                 \
                                                                                \
    static constexpr bool value = type::value;                                  \
};

#define HAS_VAR_MEMBER_WEAK(NAME, CLASS_TYPE, MEMBER_TYPE)                      \
VAR_MEMBER_WEAK(NAME)                                                           \
static_assert (has_member_weak_##NAME<CLASS_TYPE, MEMBER_TYPE>::value,          \
PREAMBLE "class [" #CLASS_TYPE "] has not member ==> "                          \
 #MEMBER_TYPE " " #NAME);


/**
 * \brief Check if a class has a member with a given name and of a given type
 * Hence the type restriction is hard
 */
#define VAR_MEMBER_HARD(NAME)                                                   \
template < typename TYPE, typename MEMBER >                                     \
struct has_member_hard_##NAME                                                   \
{                                                                               \
    template < typename T >                                                     \
    static constexpr auto                                                       \
     Check ( T* ) -> typename                                                   \
    std::is_same< decltype ( std::declval<T>( ).NAME), MEMBER>::type;           \
                                                                                \
    template<typename>                                                          \
    static constexpr std::false_type                                            \
    Check ( ... );                                                              \
                                                                                \
    typedef decltype ( Check<TYPE>( 0 ) ) type;                                 \
                                                                                \
    static constexpr bool value = type::value;                                  \
};


// Compulsory function for configurations
FUNCTION_MEMBER_WEAK(SetConfiguration);
FUNCTION_MEMBER_WEAK(Configure);
FUNCTION_MEMBER_WEAK(ReadInnerConf);
FUNCTION_MEMBER_WEAK (Aggregate);
JUST_FUNCTION_MEMBER_NAME(SetConfiguration);

// Compulsory function for interfaces
FUNCTION_MEMBER_WEAK (Wrap);
FUNCTION_MEMBER_WEAK (SetIteration);


// Compulsory functions to pass global configurer
FUNCTION_MEMBER_WEAK(GetParamInt);
FUNCTION_MEMBER_WEAK(GetParamFloat);
FUNCTION_MEMBER_WEAK(GetParamStr);
FUNCTION_MEMBER_WEAK(GetListInt);
FUNCTION_MEMBER_WEAK(GetListFloat);
FUNCTION_MEMBER_WEAK(GetListStr);

} // namesapce gnsm
#endif	/* INTERFACECONSTRINTS_H */