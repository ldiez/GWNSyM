#ifndef TYPESYSTEMCONSTRAINT_H
#define	TYPESYSTEMCONSTRAINT_H

#include "InterfaceConstraints.h"

namespace gnsm
{
namespace ts
{

#define TS_HAS_TYPE_CONFIGURATION_SETTER(CLASS_TYPE, CLASS_CONF)                \
static_assert(                                                                  \
has_function_weak_SetConfiguration<CLASS_TYPE, void, CLASS_CONF & >::value,     \
PREAMBLE "===>>> Type class does not have the proper configuration setter.\n"   \
"Required interface is (or convertible to): \n"                                 \
"[ void SetConfiguration ( CLASS_CONF const& ) ]"                               \
);

#define TS_HAS_NOT_TYPE_CONFIGURATION_SETTER(CLASS_TYPE)                        \
static_assert(                                                                  \
!has_just_function_SetConfiguration<T>::value,                                  \
PREAMBLE "===>>> A type without an associated configurer cannot have "          \
"a function called SetConfiguration"                                            \
);

#define TS_ITEM_CONSTRUCTIBLE(CLASS_TYPE)                                       \
static_assert(                                                                  \
std::is_constructible<CLASS_TYPE, Id_t>::value ||                               \
std::is_constructible<CLASS_TYPE>::value,                                       \
PREAMBLE "===>>> A type is passed that is not constructible either by passing " \
"an Id_t or by default"                                                         \
);

} // namespace ts 
} // namespace gnsm

#endif	/* TYPESYSTEMCONSTRAINT_H */