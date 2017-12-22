#ifndef SERVICEALWAYSON_H
#define SERVICEALWAYSON_H

#include "Service.h"

/**
 * \description Silly class to set the service always with demand
 */
class ServiceAlwaysOn
{
public:

    /**
     * \brief Set a sequence of intentions. If no sequence is passed around
     * the service has always ACTIVE intention. 
     */

    ServiceAlwaysOn(std::vector<Intention> inten = std::vector<Intention>())
    : m_intentions(inten)
    {
    }

    void SetIteration(std::uint32_t iter)
    {
        m_iter = iter;
    }

    void operator()(gnsm::Ptr_t<Service> serv)
    {
        if (m_intentions.size() < m_iter)
        {
            serv->SetIntention(Intention::ACTIVE);
        }
        else
        {
            serv->SetIntention(m_intentions.at(m_iter - 1));
        }
    }
private:
    std::vector<Intention> m_intentions;
    std::uint32_t m_iter;

};

#endif /* SERVICEALWAYSON_H */