#ifndef NAMESEARCH_H
#define	NAMESEARCH_H

#include <regex>

namespace gnsm 
{
namespace ts
{

static const std::regex KnotSeparator_c ( "::" );
static const std::sregex_token_iterator EndOfSequence_c;

bool CheckNames ( std::string const& stored, std::string const& passed )
{
    std::sregex_token_iterator ts_(stored.begin(), stored.end(), KnotSeparator_c, -1);
    std::sregex_token_iterator tp_(passed.begin(), passed.end(), KnotSeparator_c, -1);
    std::vector<std::string> vs_;
    std::vector<std::string> vp_;

    while ( ts_ != EndOfSequence_c )
    {
        vs_.push_back(*ts_);
        ++ts_;
    }

    while ( tp_ != EndOfSequence_c )
    {
        vp_.push_back(*tp_);
        ++tp_;
    }

    if ( vs_.size() != vp_.size() )
    {
        return false;
    }

    for ( auto i = 0u; i < vs_.size(); ++i )
    {
        if ( (isdigit((int) vs_[i][0]) && vp_[i] != "*" && vp_[i] != vs_[i]) 
                || (!isdigit((int) vs_[i][0]) && vp_[i] != vs_[i]))
        {
            return false;
        }
    }
    return true;
}

} // namespace ts
} // namespace gnsm 

#endif	/* NAMESEARCH_H */