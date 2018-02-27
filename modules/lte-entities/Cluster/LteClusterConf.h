#ifndef LTECLUSTERCONF_H
#define LTECLUSTERCONF_H

#include <string>
#include "CoreUtils.h"

class LteClusterConf
{
public:
    LteClusterConf ( std::string const& path, std::string const& name = "CLUSTER");
    
    gnsm::InnerItemsCounterList_t ReadInnerConf ( void ) const;
    
    std::string GetClusterName (void ) const;
private:
    std::uint32_t m_macroNumber;
    std::uint32_t m_picoNumber;
    const std::string m_cName;
};

#endif /* LTECLUSTERCONF_H */

