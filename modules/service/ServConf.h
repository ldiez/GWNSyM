#ifndef SERVCONF_H
#define SERVCONF_H

#include <string>
#include "Traffic.h"
#include "ServCommon.h"

class ServConf
{
public:

    ServConf(ServType st, std::string const& fileName);

    /**
     * \brief Provide the capacity demand of the service down-link
     * \return <-- Capacity demand
     */
    Traffic GetDlCapacity(void) const;

    /**
     * \brief Provide the capacity demand of the service up-link
     * \return <-- Capacity demand
     */
    Traffic GetUlCapacity(void) const;

    /**
     * \brief Give the type of the service
     * \return <-- Service type
     */
    ServType GetType(void) const;

    /**
     * \brief Give the name of the service type (string)
     * \return <-- Service name
     */
    std::string GetTypeName(void) const;
private:
    ServType m_type;
    Traffic m_dlCapacity;
    Traffic m_ulCapacity;
};

#endif /* SERVCONF_H */