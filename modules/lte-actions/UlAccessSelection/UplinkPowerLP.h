#ifndef UPLINKPOWERLP_H
#define UPLINKPOWERLP_H

#include "CoreUtils.h"
#include <glpk.h> 
#include <map>
class User;
/**
 * \problem
 * minimize \sum_{i \in U}{x_i}
 * s.t. 
 *      x_i - \sum_{j != i}{B_{ij} x_j} <= A_i     
 *      0 <= x_i <= max_i
 * 
 * A_i = SINRth_i \cdot \sigma^2*NF_{\beta(i)}/L_{i\beta(i)}
 * B_{ij} = SINRth_i N_j r_{ij}/{N_{\beta(j)} L_{j\beta(i) L_{i\beta(i)}} } 
 */
class UplinkPowerLP {
public:
    UplinkPowerLP();
    ~UplinkPowerLP();
    void operator()(gnsm::Vec_t<User> users);
private:
    void Clear ();
    void FilterUsers (gnsm::Vec_t<User> users);
    void Map2problem ();
    
    double GetA (gnsm::Ptr_t<User> u) const;
    double GetB (gnsm::Ptr_t<User> us, gnsm::Ptr_t<User> ui) const;
    
    void AddRows(void);
    void AddCols(void);
    void SetCoefs(void);
    void Solve (void);
    void SetPower (void);
    
    void Print (void);
    glp_prob* m_lp;
    
    gnsm::Vec_t<User> m_targetUsers;
    
    std::vector<double> m_vecAs;
    std::vector<double> m_coefs;
    std::vector<double> m_max;
    std::vector<double> m_sol;
    std::vector<std::vector<double>> m_matBs;
};

#endif /* UPLINKPOWERLP_H */