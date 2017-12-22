#ifndef FILTERINTERFACECONSTRAINT_H
#define	FILTERINTERFACECONSTRAINT_H

#include "InterfaceConstraints.h"
namespace gnsm
{


/*
 * \brief A filter is to be convertible to a function returning bool and taking a reference or
 * const reference to the item
 */
#define HAS_FILTER_INTERFACE(FILTER, ITEM)                                                      \
static_assert(                                                                                  \
std::is_convertible<FILTER, std::function<bool(ITEM const&)>>::value ||                         \
std::is_convertible<FILTER, std::function<bool(ITEM &)>>::value,                                \
PREAMBLE "===>>> Filter does not provide the proper interface.\n"                               \
);

#define HAS_INTERFACE_INTERFACE(INTERFACE, ITEM)                                                \
static_assert(                                                                                  \
std::is_constructible<INTERFACE>::value &&                                                      \
has_function_weak_Wrap<INTERFACE, void, ITEM&>::value,                                          \
PREAMBLE "===>>> The interface must be default constructible and it must have the"              \
"following interface [void Wrap( std::shared_ptr<INTEM>)], or convertible\n"                    \
);

} // namespace gnsm 

#endif	/* FILTERINTERFACECONSTRAINT_H */