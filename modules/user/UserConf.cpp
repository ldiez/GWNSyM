#include "UserConf.h"
#include "Configurer.h"
#include "Log.h"

LOG_REGISTER_MODULE("UserConf");

gnsm::InnerItemsCounterList_t
UserConf::ReadInnerConf(void) const
{
    BEG END;
    return
    {
        {
            "LTE_UE", 1
        },
        {
            "GENERIC_SERVICE", 1
        },
    };
}








