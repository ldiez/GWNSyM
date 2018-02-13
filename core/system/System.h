#ifndef SYSTEM_H
#define SYSTEM_H

#include "CoreUtils.h"
#include "GlobalConfig.h"
#include "ConfigurerConstraints.h"

#include "detail/SystemUtils.h"
#include "detail/type-system/AssetManager.h"
#include "detail/GenericAction.h"
#include "detail/FilterAndInterfaceHelper.h"
#include "detail/ActionConstraint.h"
#include "detail/SystemImpl.h"

namespace gnsm
{

/**
 * \brief main class to store the system deployment. 
 * This class will be in charge of connecting all the system components and the actions 
 * defined by the user.
 * 
 * \endnotes
 */

class System
{
public:

    /**
     * \brief Set global configurer. The configurer is to have the appropriate interface.
     * Once the conf is passed it is used to perform the proper configuration, so the 
     * additional configuration parameters must have been defined before passing it
     * \return <-- Self reference
     */
    template < typename CONF, typename... ARGS >
    System& SetConfig(ARGS&&... args);

    /**
     * \brief Add new user defined global configuration. The parameters here are restricted to:
     * - Path for total time (GLOB_CONF::TOTAL_TIME) 
     * - Path for snapshot time (GLOB_CONF::SNAPSHOT_TIME)
     * \param i --> Enum to indicate the parameter (total or snapshot)
     * \param path --> Path to the parameter
     * \return <-- Self reference
     */
    System& AddGlobalConfigParam(GLOB_CONF const& i, NamePath_t&& path);

    /**
     * \brief Add new user defined global configuration to define the number 
     * of elements of a final type to be created:
     * types created by network, so not belonging to any other type.
     * \param finalName --> Name  that must match to one of the types defined 
     * \param path --> Path to the number of the particular final types
     * \return <-- Self reference
     */
    System& AddGlobalConfigFinal(std::string&& finalName, NamePath_t&& path);
    System& SetTreeBase(std::string&& finalName, NamePath_t&& path);

    /**
     * \brief Interface of the asset manager to forward the registered types
     * \return <-- Self reference
     */
    template < typename TYPE, typename CONF = ts::EMPTY, typename... CONF_ARGS >
    System& AddType(std::string&& name, CONF_ARGS&&... confPath);
    template < typename TYPE, typename CONF = ts::EMPTY, typename... CONF_ARGS >
    System& Type(std::string&& name, CONF_ARGS&&... confPath);

    /**
     * \brief Create a new set as an aggregation of existing ones
     * \param name --> Name of the set
     * \param strVec --> List of existing sets to e aggregated
     * \return <-- Self reference
     */
    System& Aggregate(std::string&& name, std::vector<std::string> const& strVec);

    /**
     * \brief Add a new updater class that will be called only at the beginning of the experiment. 
     * It can be used to locate elements that won't move (such as base stations)
     * \param name --> Name of the set to be updated
     * \param args --> Arguments to build the class
     * \return <-- Self reference
     */
    template < typename C, typename... ARGS >
    System& Initializer(OneSet name, ARGS&&... args);
    
    template < typename C, typename... ARGS >
    System& Initializer(TwoSets name, ARGS&&... args);
    
    template < typename C, typename... ARGS >
    System& Initializer(ThreeSets name, ARGS&&... args);

    /**
     * \brief Add a new clasp class that will be called at the end of the experiment.
     * It can be used to take statistics at the end
     * \param name --> name of the set to be clasped
     * \param args --> Arguments to build the class
     * \return  <-- Self reference
     */
    
    template < typename C, typename... ARGS >
    System& Finalizer(OneSet name, ARGS&&... args);
    
    template < typename C, typename... ARGS >
    System& Finalizer(TwoSets name, ARGS&&... args);
    
    template < typename C, typename... ARGS >
    System& Finalizer(ThreeSets name, ARGS&&... args);

    /**
     * \brief Instantiate action from a callable class to be called iteratively with different 
     * number of names of sets
     * \param name(s)
     * \param args--> Arguments to build the class
     * \return <-- Self reference
     */
    template < typename C, typename... ARGS > System&
    Action(OneSet name, ARGS&&... args);

    template < typename C, typename... ARGS> System&
    Action(TwoSets names, ARGS&&... args);

    template < typename C, typename... ARGS> System&
    Action(ThreeSets names, ARGS&&... args);

    /**
     * \brief Create a new set of elements from a previous one and applying a filter over the
     * elements. The filter instance is internally constructed (default constructible). Note 
     * that the filtering condition is NOT re-evaluated in following iterations.
     * \param setName --> Name of the previous set
     * \param newName --> Name of the new one
     * \return <-- Self reference
     */
    template < typename F >
    System& Filter(std::string const& setName, std::string&& newName);

    /**
     * \brief Create a new set of elements from a previous one and applying a filter over the
     * elements. The filter instance is passed around. Note 
     * that the filtering condition is NOT re-evaluated in following iterations.
     * \param f --> Filter instance
     * \param setName --> Name of the previous set
     * \param newName --> Name of the new one
     * \return <-- Self reference
     */
    template < typename F >
    System& Filter(F const&f, std::string const& setName, std::string&& newName);

    /**
     * \brief Create a new set of elements which will act as a user defined interface for 
     * the original elements. The interface/wrapper type is indicated in the template argument and
     * it must be constructible with a gnsm::Ptr of the inner type.
     * \param setName --> Name of the previous set
     * \param newName --> Name of the new originated set
     * \return <-- Self reference
     */
    template < typename W >
    System& Interface(std::string const& setName, std::string&& newName);

    /**
     * \brief Deploy the current network global configuration configuration 
     * \return <-- Self reference
     */
    System& Deploy(void);

    /**
     * \brief Print a tree with the types instantiated
     * \param name --> Set whether the full name or the short one is shown
     * \return --
     */
    void PrintTree(ts::NAME const& name = ts::NAME::FULL) const;

    /**
     * \brief Print a tree with the types instantiated and their numbers
     * \param name --> Set whether the full name or the short one is shown
     * \return --
     */
    void PrintInstances(ts::NAME const& name = ts::NAME::FULL) const;
    void PrintTreeInstances(ts::NAME const& name = ts::NAME::FULL) const;

    /**
     * \brief Print information of all the instances without the network
     * \param name --> Set whether the full name or the short one is shown
     * \return --
     */
    void PrintAll(ts::NAME const& name = ts::NAME::FULL) const;

    /**
     * \brief Iteratively run all the registered actions
     * \return --
     */
    void Run(void);

private:

    template < typename CONF, typename... ARGS > void
    SetConfig_impl(ARGS&&... args);

    std::uint32_t m_totalTime;
    std::uint32_t m_snapshotStepTime;
    std::uint32_t m_totalSnapshots;

    GlobalConfig m_globConfig;

    ts::AssetManager m_am;

    detail::ActionsVector_t m_actions;
    detail::ActionsVector_t m_initializers;
    detail::ActionsVector_t m_finalizers;
    detail::LocalItems_t m_localSets;
    detail::PersistentFilters_t m_perFilters;
};

/*************************************************************************************************/

template < typename CONF, typename... ARGS > System&
System::SetConfig(ARGS&&... args)
{
    HAS_CONFIGURER_INTERFACE(CONF);
    SetConfig_impl<CONF>(std::forward<ARGS>(args)...);
    return *this;
}

template < typename TYPE, typename CONF, typename... CONF_ARGS > System&
System::AddType(std::string&& name, CONF_ARGS&&... confArgs)
{
    HAS_CORRECT_CONF_PARAMS(CONF, CONF_ARGS...);
    m_am.AddType<TYPE, CONF>(std::forward<std::string>(name),
            std::forward<CONF_ARGS>(confArgs)...);
    return *this;
}

template < typename TYPE, typename CONF, typename... CONF_ARGS > System&
System::Type(std::string&& name, CONF_ARGS&&... confArgs) {
    HAS_CORRECT_CONF_PARAMS(CONF, CONF_ARGS...);
    m_am.AddType<TYPE, CONF>(std::forward<std::string>(name),
            std::forward<CONF_ARGS>(confArgs)...);
    return *this;
}

template < typename C, typename... ARGS > System&
System::Initializer(OneSet name, ARGS&&... args)
{
    HAS_CORRECT_ACTION_PARAMS(C, ARGS...);
    detail::DoApplyAction1<C, ARGS...>
    (m_initializers, m_am, m_localSets, name.m_name, std::forward<ARGS>(args)...);
    return *this;
}

template < typename C, typename... ARGS > System&
System::Initializer(TwoSets names, ARGS&&... args)
{
    HAS_CORRECT_ACTION_PARAMS(C, ARGS...);
    detail::DoApplyAction2<C, ARGS...>(m_initializers, m_am, m_localSets, names.m_name1,
            names.m_name2, std::forward<ARGS>(args)...);
    return *this;
}

template < typename C, typename... ARGS> System&
System::Initializer(ThreeSets names, ARGS&&... args)
{
    HAS_CORRECT_ACTION_PARAMS(C, ARGS...);
    detail::DoApplyAction3<C, ARGS...>(m_initializers, m_am, m_localSets, names.m_name1,
            names.m_name2, names.m_name3, std::forward<ARGS>(args)...);
    return *this;
}

template < typename C, typename... ARGS > System&
System::Finalizer(OneSet name, ARGS&&... args)
{
    HAS_CORRECT_ACTION_PARAMS(C, ARGS...);
    detail::DoApplyAction1<C>(m_finalizers, m_am, m_localSets, name.m_name,
            std::forward<ARGS>(args)...);
    return *this;
}

template < typename C, typename... ARGS > System&
System::Finalizer(TwoSets names, ARGS&&... args)
{
    HAS_CORRECT_ACTION_PARAMS(C, ARGS...);
    detail::DoApplyAction2<C, ARGS...>(m_finalizers, m_am, m_localSets, names.m_name1,
            names.m_name2, std::forward<ARGS>(args)...);
    return *this;
}

template < typename C, typename... ARGS> System&
System::Finalizer(ThreeSets names, ARGS&&... args)
{
    HAS_CORRECT_ACTION_PARAMS(C, ARGS...);
    detail::DoApplyAction3<C, ARGS...>(m_finalizers, m_am, m_localSets, names.m_name1,
            names.m_name2, names.m_name3, std::forward<ARGS>(args)...);
    return *this;
}

template < typename C, typename... ARGS> System&
System::Action(OneSet name, ARGS&&... args)
{
    HAS_CORRECT_ACTION_PARAMS(C, ARGS...);
    detail::DoApplyAction1<C, ARGS...>
            (m_actions, m_am, m_localSets, name.m_name, std::forward<ARGS>(args)...);
    return *this;
}

template < typename C, typename... ARGS > System&
System::Action(TwoSets names, ARGS&&... args)
{
    HAS_CORRECT_ACTION_PARAMS(C, ARGS...);
    detail::DoApplyAction2<C, ARGS...>(m_actions, m_am, m_localSets, names.m_name1,
            names.m_name2, std::forward<ARGS>(args)...);
    return *this;
}

template < typename C, typename... ARGS> System&
System::Action(ThreeSets names, ARGS&&... args)
{
    HAS_CORRECT_ACTION_PARAMS(C, ARGS...);
    detail::DoApplyAction3<C, ARGS...>(m_actions, m_am, m_localSets, names.m_name1,
            names.m_name2, names.m_name3, std::forward<ARGS>(args)...);
    return *this;
}

template < typename F > System&
System::Filter(std::string const& setName, std::string&& newName)
{
    detail::DoApplyFilter<F>(m_localSets, m_am, F(), setName,
            std::forward<std::string>(newName));
    return *this;
}

template < typename F > System&
System::Filter(F const&f, std::string const& setName, std::string&& newName)
{
    detail::DoApplyFilter<F>(m_localSets, m_am, f, setName,
            std::forward<std::string>(newName));
    return *this;
}

template < typename W > System&
System::Interface(std::string const& setName, std::string&& newName)
{

    detail::DoApplyInterface<W>(m_localSets, m_am, setName,
            std::forward<std::string>(newName));
    return *this;
}

template < typename CONF, typename... ARGS > void
System::SetConfig_impl(ARGS&&... args)
{
    CONF conf_(std::forward<ARGS>(args)...);
    m_totalTime = conf_.GetParamInt(m_globConfig.GetTotalTimePath());
    m_snapshotStepTime = conf_.GetParamInt(m_globConfig.ReadSnapshotTimePath());
    m_totalSnapshots = m_totalTime / m_snapshotStepTime;

    for (auto& item_ : m_globConfig.ReadFinalNamesList())
    {

        auto num_ = conf_.GetParamInt(m_globConfig.ReadFinalPath(item_));
        m_globConfig.SetNumberOf(item_, num_);
    }
}

} // namespace gnsm
#endif /* SYSTEM_H */