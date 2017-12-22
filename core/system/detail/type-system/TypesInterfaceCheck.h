#ifndef TYPESINTERFACECHECK_H
#define	TYPESINTERFACECHECK_H

#include "TypeUtils.h"
#include "TypeSystemConstraint.h"

namespace gnsm
{
namespace ts
{

/**
 * \description Interface checking for the types passed to the asset manager
 * \detail If a configuration class is defined for the type
 * - A warning is shown if it does not have configuration setter
 */
template < typename T, typename CONFIG >
struct CheckInterface
{
    TS_ITEM_CONSTRUCTIBLE(T);
    TS_HAS_TYPE_CONFIGURATION_SETTER(T, CONFIG);
};

template < typename T >
struct CheckInterface<T, ts::EMPTY>
{
    TS_ITEM_CONSTRUCTIBLE(T);
    TS_HAS_NOT_TYPE_CONFIGURATION_SETTER ( T );
};

} // namespace ts
} // namespace gnsm 
#endif	/* TYPESINTERFACECHECK_H */