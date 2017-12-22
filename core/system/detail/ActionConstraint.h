#ifndef ACTIONCONSTRAINT_H
#define	ACTIONCONSTRAINT_H

#include "InterfaceConstraints.h"

namespace gnsm
{
namespace detail
{

#define IS_DEFAULT_ACTION(TYPE)                                                 \
static_assert(                                                                  \
is_just_callable<TYPE>::value && std::is_constructible<TYPE>::value,            \
PREAMBLE "===>>> Action must me trivially constructible and callable "          \
);

} // namesapce detail
} // namespace gnsm
#endif	/* ASTCONSTRAINTS_H */

