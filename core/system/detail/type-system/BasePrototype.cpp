#include "BasePrototype.h"
#include "Log.h"

LOG_REGISTER_MODULE("ts::BasePrototype");

namespace gnsm
{
namespace ts
{

BasePrototype::BasePrototype ( std::string&& name ) : m_name ( name )
{
    BEG END;;
}

InnerItemsCounterList_t 
BasePrototype::GetInnerList ( void ) const
{
    BEG END;
    return DoGetInnerList();
}

std::string const&
BasePrototype::ReadName ( void ) const
{
    BEG END;
    return m_name;
}

BaseType::BaseTypePtr_t 
BasePrototype::GetInstance ( void ) const
{
    BEG END;
    return DoGetInstance ();
}

} // namesapce ts
} // namespace gnsm