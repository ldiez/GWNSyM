#ifndef GLOBALCONFIG_H
#define	GLOBALCONFIG_H

#include <unordered_map>
#include <list>
#include "CoreUtils.h"

namespace gnsm
{

enum GLOB_CONF : std::uint8_t
{
    TOTAL_TIME = 1, SNAPSHOT_TIME = 2
};

/**
 * \description This class stores a list of parameters and the number of elements to be created
 * of each. 
 * It is used by Network to store the names of the Final Types to be read in the global 
 * configuration and the number of them that are to be created.
 */
struct GlobalConfig
{
    /**
     * \brief Register a new parameter name
     * \param name --> Name of the new parameter
     * \param path --> Path to get the parameter in the configuration
     * \return --
     */
    void AddFinalType ( std::string&& name, NamePath_t&& path );

    /**
     * \brief Set the number of elements for a given type. 
     * \param name --> Name 
     * \param n --> Number 
     * \return --
     */
    void SetNumberOf ( std::string const& name, std::uint32_t n );

    /**
     * \brief Provide the number of elements for a given name
     * \param name --> Name
     * \return <-- Number 
     */
    std::uint32_t NumberOf ( std::string const& name );

    /**
     * \breif Set a user defined path to get the total time of the experiment
     * \param path --> Path to the total time parameter
     * \return --
     */
    void SetTotalTimePath ( NamePath_t&& path );

    /**
     * \brief Set a user defined path for the snapshot time 
     * \param path --> Path to the snapshot parameter
     * \return -- 
     */
    void SetSnapshotTimePath ( NamePath_t&& path );
    
    /**
     * \brief Give the path to the total time
     * \return <-- Parameter path
     */
    NamePath_t GetTotalTimePath ( void );
    
    /**
     * \brief Give the path to the snapshot time
     * \return <-- Parameter path
     */
    NamePath_t const& ReadSnapshotTimePath ( void ) const;
    
    /**
     * \brif Give the list of final names that have been registered
     * \return <-- List
     */
    std::list<std::string> const& ReadFinalNamesList ( void ) const;
    
    /**
     * \brief Give registered path for the number of elements to be deployed by a final
     * \param name --> Final name
     * \return <-- Path
     */
    NamePath_t const& ReadFinalPath ( std::string const& name) const;

private:
    
    NamePath_t m_totalTime = { "TIME", "TOTAL" };
    NamePath_t m_snapshotTime = { "TIME", "SNAPSHOT" };

    using FinalNumMap_t = std::unordered_map< std::string, std::uint32_t >;
    using FinalPathMap_t = std::unordered_map< std::string, NamePath_t >;

    FinalPathMap_t     m_finalPaths;
    FinalNumMap_t   m_finalNumber;
    std::list<std::string> m_finalNamesList;
};

} // namespace gnsm

#endif	/* GLOBALCONFIG_H */

