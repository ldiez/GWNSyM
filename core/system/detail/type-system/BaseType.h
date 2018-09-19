#ifndef BASETYPE_H
#define	BASETYPE_H

#include <memory>
#include <cassert>
#include <functional>

#include "KnotName.h"
#include "TypeUtils.h"
#include "Log.h"

namespace gnsm
{
namespace ts
{

enum class NAME : std::uint8_t
{
    SHORT = 0,
    FULL
};

struct BaseType
{

    using BaseTypePtr_t = std::shared_ptr<BaseType>;
    using Instances_t = std::unordered_map<Id_t, BaseTypePtr_t>;
    using InnerTypes_t = std::unordered_map<KnotName, Instances_t>;
    using BackRegister_t = std::function<void(std::string, BaseTypePtr_t )>;

    /**
     * \brief Ctor
     * \param name --> Name of the type
     */
    BaseType ( std::string name, BackRegister_t br );
    virtual ~BaseType ( ) = default;

    /**
     * \brief Read the position of the type within the types tree
     * \return --> Reading access to the full name
     */
    KnotName const& ReadFullName ( void ) const;

    /**
     * \brief Read the name registered for the associated underlying type
     * \return --> Reading access to the name
     */
    std::string const& ReadName ( void ) const;

    /**
     * \brief Store an inner type within the type
     * \param ptr -> Pointer to the inner type
     * \return --
     */
    void AddInner ( BaseTypePtr_t ptr, std::uint32_t num );

    /**
     * \brief Create a number of elements of instances of the underlying type
     * \param num --> Number of elements
     * \return --
     */
    void Create ( std::uint32_t num, std::string str = std::string ( " " ) );

    /**
     * \brief Make a type cloning. It does not clone the items as they are user defined and no
     * assumption can be done over the copy constructor of assignment operator
     * \return <-- Pointer to the new type
     */
    BaseTypePtr_t Clone ( void );

    /**
     * \brief Check whether the type has inner types (it makes sense after the deployment)
     * \return 
     */
    bool HasInners ( void ) const;

    /**
     * \brief Recursively print (std output) the inner types names, adding an extra "  " 
     * for each innermost knot 
     * \param space --> Initial spacing 
     * \param name --> Set short or full name 
     * \return --
     */
    void PrintInners ( std::string space, NAME const& name ) const;

    /**
     * \brief
     * \param space
     * \param name
     */
    virtual void PrintInstancesCounter ( std::string space, NAME const& name ) const = 0;

    /**
     * \brief
     * \param space
     * ºparam name
     */
    virtual void PrintAllContent ( std::string space, NAME const& name ) const = 0;



protected:

    using InnerRefTypes_t = std::unordered_map<KnotName, BaseTypePtr_t>;
    using Creation_t = std::unordered_map<std::string, std::uint32_t>;

    // Implementation of the interface
    virtual BaseTypePtr_t DoClone ( void ) = 0;
    virtual void DoCreate ( std::uint32_t, std::string str = " " ) = 0;

    /**
     * \brief Add a level to the position in the tree
     * \param name Level name 
     * \return --
     */
    void PushFrontKnot ( KnotName const& tree );

    Creation_t m_creation;
    InnerRefTypes_t m_innerRefTypes;
    const std::string m_name;
    KnotName m_knotName;

    InnerTypes_t m_innerTypes;

    BackRegister_t m_br;
};

} // namespace ts
} // namespace gnsm

#endif	/* BASETYPE_H */