#ifndef PROTOTYPE_H
#define	PROTOTYPE_H

#include <memory>

#include "BasePrototype.h"
#include "Type.h"

namespace gnsm
{
namespace ts
{

template < typename TYPE, typename CONF = EMPTY >
class Prototype : public BasePrototype
{
    using Type_t = TYPE;
    using Conf_t = CONF;
public:

    template < typename... CONF_ARGS >
    Prototype ( std::string&& name, BaseType::BackRegister_t br, CONF_ARGS&&... confArgs ) :
    BasePrototype ( std::forward<std::string>( name ) )
    , m_conf ( std::make_shared<Conf_t>( std::forward<CONF_ARGS>(confArgs)... ) )
    , m_br ( std::move ( br ) ) { }

private:

    InnerItemsCounterList_t DoGetInnerList ( void ) const final
    {
        return GenericGetInnerList(m_conf);
    }

    BaseType::BaseTypePtr_t DoGetInstance ( void ) const final
    {
        auto newType_ = BaseType::BaseTypePtr_t(std::make_shared<Type<Type_t, Conf_t>>( m_name, m_conf, m_br ));
        return newType_;
    }


    std::shared_ptr<Conf_t> m_conf;
    BaseType::BackRegister_t m_br;
};

} // namespace ts
} // namespace gnsm

#endif	/* PROTOTYPE_H */