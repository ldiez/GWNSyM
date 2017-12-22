#ifndef KNOTNAME_H
#define	KNOTNAME_H

#include <list>
#include <regex>

namespace gnsm
{
namespace ts
{

using KnotList_t = std::list<std::string>;
/**
 * \brief Conversion from list to a proper string
 * \param list --> Lis of knots
 * \return <-- String with the full knot-path
 */
inline std::string KnotListToString ( KnotList_t const& list )
{
    auto str_ = std::string();

    for ( auto item_ = list.begin(); item_ != list.end(); ++item_ )
    {
        auto aux_ = ( item_ != list.begin() ? "::" + *item_ : *item_ );
        str_ += aux_;
    }
    return str_;
}

class KnotName
{
public:

    KnotName ( KnotList_t const& list ) : m_list ( list ), m_listStr(KnotListToString(list))
    {
    }
    
    KnotName ( KnotName const& other )
    {
        m_list = other.m_list;
        m_listStr = other.m_listStr;
    }

    explicit operator std::string ( ) const
    {
        return m_listStr;
    }

    std::string const& ReadStr ( void ) const
    {
        return m_listStr;
    }
    
    std::string const& ReadLast ( void ) const 
    {
        return m_list.back();
    }
    
    void PopFront ( void )
    {
        m_list.pop_front();
        m_listStr = KnotListToString(m_list);
    }
    
    void PopBack ( void )
    {
        m_list.pop_back();
        m_listStr = KnotListToString(m_list);
    }
    
    void PushFront (std::string const& other )
    {
        m_list.push_front(other);
        m_listStr = KnotListToString(m_list);
    }
    
    void PushBack (std::string const& other )
    {
        m_list.push_back(other);
        m_listStr = KnotListToString(m_list);
    }
    
    void PushBack (KnotName const& other )
    {
        m_list.insert(m_list.end(), other.m_list.begin(), other.m_list.end());
        m_listStr = KnotListToString(m_list);
    }
    
    void PushFront (KnotName const& other )
    {
        m_list.insert(m_list.begin(), other.m_list.begin(), other.m_list.end());
        m_listStr = KnotListToString(m_list);
    }
    
    bool HasCycle ( void ) const
    {
        for (auto item_ = m_list.begin(); item_ != m_list.end(); ++item_)
        {
            auto itemAux_ = item_;
            for (auto item2_ = ++itemAux_; item2_ != m_list.end(); ++item2_ )
            {
                if (*item2_ == *item_)
                {
                    return true;
                }
            }
        }
        return false;
    }

private:
    KnotList_t m_list;
    std::string m_listStr;    
};
} // namespace ts
} // namespace gnsm

namespace std
{

template<> struct less<gnsm::ts::KnotName>
{

    bool operator() ( gnsm::ts::KnotName const& lhs, gnsm::ts::KnotName const& rhs ) const noexcept
    {
        return (lhs.ReadStr() < rhs.ReadStr() );
    }
};

template<> struct equal_to<gnsm::ts::KnotName>
{

    bool operator() ( gnsm::ts::KnotName const& lhs, gnsm::ts::KnotName const& rhs ) const noexcept
    {
        return (lhs.ReadStr() == rhs.ReadStr() );
    }
};

template<> struct hash<gnsm::ts::KnotName>
{

    std::size_t operator() ( gnsm::ts::KnotName const& kn ) const noexcept
    {
        return (std::hash<std::string>()(kn.ReadStr()) );
    }
};
}


#endif	/* KNOTNAME_H */