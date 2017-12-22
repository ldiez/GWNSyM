#ifndef BASEPROTOTYPE_H
#define	BASEPROTOTYPE_H

#include "BaseType.h"

namespace gnsm
{
namespace ts
{

/**
 * \description This class serves as a base class for all the prototypes 
 * registered within the types system
 */
class BasePrototype
{
public:
    BasePrototype ( std::string&& name );
    virtual ~BasePrototype ( ) = default;
    
    /**
     * \brief Provide a list of inner types 
     * \return --> Inner types list
     */
    InnerItemsCounterList_t GetInnerList ( void ) const;
    
    /**
     * \brief Provide the prototype name 
     * \return <-- Name
     */
    std::string const& ReadName ( void ) const;
    
    /**
     * \brief Provide an instance of the actual type
     * \return --> Type pointer
     */
    BaseType::BaseTypePtr_t GetInstance ( void ) const;
    
    
protected:
    // interface implementation
    virtual InnerItemsCounterList_t DoGetInnerList ( void ) const  = 0;
    virtual BaseType::BaseTypePtr_t DoGetInstance ( void ) const = 0;
    std::string m_name;
};

using BaseProtoPtr_t = std::unique_ptr<BasePrototype>;

} // namespace ts 
} // namespace gnsm

#endif	/* BASEPROTOTYPE_H */