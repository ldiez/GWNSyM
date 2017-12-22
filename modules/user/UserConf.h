#ifndef USERCONF_H
#define	USERCONF_H

#include "CoreUtils.h"

class UserConf
{
public:
    /**
     * \brief Pass configuration to the system 
     * \return <-- List of inner types
     */
    gnsm::InnerItemsCounterList_t ReadInnerConf ( void ) const;
    
private:
    
};

#endif	/* USERCONF_H */