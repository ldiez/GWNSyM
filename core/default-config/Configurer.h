#ifndef CONFIGURER_H
#define	CONFIGURER_H

#include <memory>
#include "ConfigFile.h"

namespace gnsm 
{

class Configurer
{
    using NamePath_t = std::vector<std::string>;

public:
    Configurer ( ) = default;
    Configurer ( std::string const& path );
    /**
     * \brief Destructor
     */
    ~Configurer ( );

    
    int GetParamInt ( NamePath_t namePath );
    
    double GetParamFloat ( NamePath_t namePath );
    
    std::string GetParamStr ( NamePath_t namePath );

    int GetParamInt ( NamePath_t namePath, int defVal );
    
    double GetParamFloat ( NamePath_t namePath, double defVal );
    
    std::string GetParamStr ( NamePath_t namePath, std::string defVal );

    std::vector<int> GetListInt ( NamePath_t namePath );
    
    std::vector<double> GetListFloat ( NamePath_t namePath );
    
    std::vector<std::string> GetListStr ( NamePath_t namePath );

    std::vector<int> 
    GetListInt ( NamePath_t namePath, std::vector<int> defVal );
    
    std::vector<double> 
    GetListFloat ( NamePath_t namePath, std::vector<double> defVal );
    
    std::vector<std::string> 
    GetListStr ( NamePath_t namePath, std::vector<std::string> defVal );


private:
    ConfigFile     m_configFile;
} ;

} // namespace gnsm
#endif	/* CONFIGURER_H */