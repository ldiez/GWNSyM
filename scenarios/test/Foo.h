#ifndef FOO_H
#define FOO_H

#include "User.h"
#include "Cluster/LteCluster.h"

struct OneSetPtr
{
    OneSetPtr ( std::string foo )
    {
        std::cout << "1Set Ptr created with " << foo << std::endl;
    }
    
    void operator() (gnsm::Ptr_t<User> up)
    {
        std::cout << "1Set Ptr gets called"<< std::endl;
    }
};

struct OneSetPair
{
    OneSetPair ( std::string foo )
    {
        std::cout << "1Set Pair created with " << foo << std::endl;
    }
    
    void operator() (gnsm::Pair_t<User> up)
    {
        std::cout << "1Set Pair gets called"<< std::endl;
    }
};

struct OneSetVec
{
    OneSetVec ( std::string foo )
    {
        std::cout << "1Set Vec created with " << foo << std::endl;
    }
    
    void operator() (gnsm::Vec_t<User> up)
    {
        std::cout << "1Set Vec gets called"<< std::endl;
    }
};

struct TwoSetsPtr
{
    TwoSetsPtr ( std::string foo )
    {
        std::cout << "2Sets Ptr-Ptr created with " << foo << std::endl;
    }
    
    void operator() (gnsm::Ptr_t<User> up, gnsm::Ptr_t<LteCluster> cl)
    {
        std::cout << "2Sets Ptr-Ptr gets called"<< std::endl;
    }
};

struct TwoSetsPair
{
    TwoSetsPair ( std::string foo )
    {
        std::cout << "2Sets Pair-Pair created with " << foo << std::endl;
    }
    
    void operator() (gnsm::Pair_t<User> up, gnsm::Pair_t<LteCluster> cl)
    {
        std::cout << "2Sets Pair-Pair gets called"<< std::endl;
    }
};

struct TwoSetsVec
{
    TwoSetsVec ( std::string foo )
    {
        std::cout << "2Sets Vec-Vec created with " << foo << std::endl;
    }
    
    void operator() (gnsm::Vec_t<User> up, gnsm::Vec_t<LteCluster> cl)
    {
        std::cout << "2Sets Vec-Vec gets called"<< std::endl;
    }
};

struct TwoSetsPtrPair
{
    TwoSetsPtrPair ( std::string foo )
    {
        std::cout << "2Sets Ptr-Pair created with " << foo << std::endl;
    }
    
    void operator() (gnsm::Ptr_t<User> up, gnsm::Pair_t<LteCluster> cl)
    {
        std::cout << "2Sets Ptr-Pair gets called"<< std::endl;
    }
};

struct TwoSetsPtrVec
{
    TwoSetsPtrVec ( std::string foo )
    {
        std::cout << "2Sets Ptr-Vec created with " << foo << std::endl;
    }
    
    void operator() (gnsm::Ptr_t<User> up, gnsm::Vec_t<LteCluster> cl)
    {
        std::cout << "2Sets Ptr-Vec gets called"<< std::endl;
    }
};

struct TwoSetsPairVec
{
    TwoSetsPairVec ( std::string foo )
    {
        std::cout << "2Sets Pair-Vec created with " << foo << std::endl;
    }
    
    void operator() (gnsm::Pair_t<User> up, gnsm::Vec_t<LteCluster> cl)
    {
        std::cout << "2Sets Pair-Vec gets called"<< std::endl;
    }
};

#endif /* FOO_H */

