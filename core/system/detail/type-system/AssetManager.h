#ifndef ASSETMANAGER_H
#define	ASSETMANAGER_H

#include <unordered_map>
#include "Prototype.h"
#include "TypesInterfaceCheck.h"

namespace gnsm
{
namespace ts
{

/**
 * \description Main class to store the registered types and the instances created
 * 
 */
class AssetManager
{
public:
    using Typesvect_t = std::vector<BaseType::BaseTypePtr_t>;

    /**
     * \brief Register a new type within the types system
     * \tparam TYPE: Specific type
     * \tparam CONF: Configuration type
     * \param name --> Name of the type
     * \return --
     * 
     * \details:
     * 
     * If CONF is not empty, then TYPE is to have the following interface:
     *      void SetConfiguration ( CLASS_CONF const& )
     * 
     */
    template < typename TYPE, typename CONF = EMPTY, typename... CONF_ARGS >
    void AddType ( std::string&& name, CONF_ARGS&&... confArgs )
    {
        CheckInterface<TYPE, CONF>{ };
        DoAddType<TYPE, CONF>( std::forward<std::string>( name )
                , std::forward<CONF_ARGS> ( confArgs )... );
    }

    /**
     * \brief Instantiate elements of a given type and pass around the internal instances to
     * their respective instances 
     * \param name --> name of the type
     * \param num --> Number of elements to create
     * \return --
     */
    void Create ( std::string const& name, std::uint32_t num );

    /**
     * \brief Search a given set of branches/leaves of the tree
     * \param name --> Name of the tree
     * \return <-- Vector of types containing the required items
     */
    Typesvect_t Search ( std::vector<std::string> const& strVec ) const;

    /**
     * \brief Search an aggregated set
     * \param strVec --> New of the aggregated set 
     * \return <-- Vector of types containing the required items
     */
    Typesvect_t SearchAggretated ( std::string const& name ) const;

    /**
     * \brief It created a new container as a result of the aggregation of others. Note that
     * it does not check if the types are the same. If the name already exist it does not
     * aggregate the new one
     * \param newName --> Name of the new container
     * \param strVec --> List of names of the other containers 
     */
    void Aggregate ( std::string&& newName, std::vector<std::string> const& strVec );

    /**
     * \brief Print the tree of types that have been registered so far
     * \param name --> Set if short or full name is shown
     * \return --
     */
    void PrintTree ( NAME const& name = NAME::SHORT ) const;

    /**
     *  \brief Print the number of the instances created so far
     * \param name --> Set if short or full name is shown
     * \return --
     */
    void PrintInstances ( NAME const& name = NAME::SHORT ) const;

    /**
     * \brief Print all the content within the tree
     * \param name --> Set if short or full name is shown
     * \param name 
     */
    void PrintAll ( NAME const& name = NAME::SHORT ) const;


private:
    using BaseProtoPtr_t = std::unique_ptr<BasePrototype>;
    using PrototypesMap_t = std::unordered_map<std::string, BaseProtoPtr_t>;
    using QuickSearch_t = std::unordered_map<std::string, BaseType::BaseTypePtr_t>;
    using TypesMap_t = std::unordered_map<KnotName, BaseType::BaseTypePtr_t>;
    using AggregatedContainers_t = std::unordered_map<std::string,
            std::vector<BaseType::BaseTypePtr_t>>;


    /**
     * \brief Function to register inner types when they are created so that it is not necessary
     * to perform an in-depth searching when accessing them
     * \param str --> Full name of the type
     * \param ptr --> Pointer to the type
     */
    void BackRegister ( std::string str, BaseType::BaseTypePtr_t ptr );

    // AddType implementation

    template < typename TYPE, typename CONF, typename... CONF_ARGS >
    void DoAddType ( std::string&& name, CONF_ARGS&&... confArgs )
    {
        m_protoMap.emplace(name, BaseProtoPtr_t(std::make_unique<Prototype<TYPE, CONF> >
                                                ( std::forward<std::string>( name )
                                                , std::bind(&AssetManager::BackRegister, this,
                                                            std::placeholders::_1,
                                                            std::placeholders::_2)
                                                , std::forward<CONF_ARGS>( confArgs )... )
                                                ));
    }

    /**
     * \brief Look for prototype. It asserts that the prototype is registered
     * \param name --> Type name
     * \return <-- Const iterator to the type
     */
    PrototypesMap_t::const_iterator FindPrototype ( std::string const& name ) const;

    /**
     * \brief Perform the deployment of one type
     * \param iter --> Iterator of that type
     * \return <-- Pointer to the new created type
     */
    BaseType::BaseTypePtr_t DoDeploy ( PrototypesMap_t::const_iterator const& iter );

    /**
     * \brief Deploy the inner types of a type
     * \param ptr --> Pointer of the parent type
     * \param list --> List of the inner types to be deployed 
     * \return --
     */
    void SpawnType ( BaseType::BaseTypePtr_t const& ptr, InnerItemsCounterList_t const& list );

    /**
     * \brief Register type instances by their full names
     * \param type --> Type ptr
     * return --
     */
    void Register ( BaseType::BaseTypePtr_t type );

    PrototypesMap_t m_protoMap; // map of registered prototypes
    TypesMap_t m_typeMap; // all the containers for name searching
    QuickSearch_t m_quickMap;
    AggregatedContainers_t m_aggregated;

};
} // namespace ts
} // namespace gnsm
#endif	/* ASSETMANAGER_H */