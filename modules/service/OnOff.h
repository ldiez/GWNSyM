#ifndef ONOFF_H
#define ONOFF_H

#include <random>
#include "Service.h"

/**
 * \brief Class that implements ON-OFF 
 */
class OnOff
{
public:
    OnOff(double on, double off);
    void operator()(gnsm::Ptr_t<Service> serv);
private:
    using ServicesTrack_t = std::unordered_map<gnsm::Ptr_t<Service>, std::uint32_t>;
    
    void DoUpdate (ServicesTrack_t::iterator iter);
    
    
    const double m_on;
    const double m_off;

    std::mt19937 m_gen;
    ServicesTrack_t m_track;
};
#endif /* ONOFF_H */