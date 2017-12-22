
#ifndef SCDCUTILS_H
#define SCDCUTILS_H

#include "CreSelection.h"
#include "User.h"

using SensedMap_t = std::unordered_map<gnsm::Ptr_t<LteCell>, double>;

struct TranslatedInfo
{
    gnsm::Ptr_t<User> m_self;
    std::uint32_t m_mutual = 0u;
    gnsm::Ptr_t<LteCell> m_currCell; // cell selected
    SensedMap_t m_sensed; // 
    double m_interFloor = 0.0; // interference created by the not accessible cells (mW))
    double m_pow = 0.0; // power of the serving cell
    double m_demand = 0.0; // in Kbps
    std::vector<double> m_interference; // potential interference (mW)
};

using TranslatedVec_t = std::vector<TranslatedInfo>;

struct LocalTranslator
{
    LocalTranslator(double backgroundLoad, double interLoad, units::dB picoCre);
    TranslatedVec_t Translate(gnsm::Vec_t<User> const&, gnsm::Vec_t<LteCell> const& inter);
    
    void Clear ( void );

private:
    gnsm::Vec_t<LteCell> const* m_inter;
    CreSelection m_sel;
    void DoSetPowers(LteUe::SensedValues_t const& sensed, TranslatedInfo& info);
    bool IsInterferingFloor(gnsm::Ptr_t<LteCell>const& cell) const;
    void SetDemand(gnsm::Vec_t<User> const& users, TranslatedVec_t& info);
    void SetIntPow(TranslatedVec_t& info);
    const double m_interLoadFactor;
    const double m_backLoadFactor;
};


#endif /* SCDCUTILS_H */

