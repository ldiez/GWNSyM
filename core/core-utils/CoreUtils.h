#ifndef COREUTILS_H
#define COREUTILS_H

#include <type_traits>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <memory>

namespace gnsm {

    /**
     * \brief Definition of useful methods generally used by other parts of the tool
     * but that do not belong to any of them
     */

#define DEPRECATED(func) func __attribute__ ((deprecated));

    /**
     * \brief Cast scope enumeration to its underlying type to print it out.
     */
    template < typename T >
    auto UnscopeEmum(T scoped) {
        return static_cast<typename std::underlying_type<T>::type> (scoped);
    }


    /**
     * \description Definition of identifier type
     */
    using Id_t = std::uint32_t;
    static const Id_t INVALID_ID = 0;

    using DoubleId_t = std::pair<Id_t, Id_t>;

    /**
     * \description Pointers for single items from the type system
     */
    template < typename T >
    using Ptr_t = std::shared_ptr<T>;

    /**
     * \description Id-Pointer pair for single item of the type system
     */
    template < typename T >
    using Pair_t = std::pair<const Id_t, Ptr_t<T> >;

    /**
     * \description Map for the items from the type system
     */
    template < typename T>
    using Map_t = std::unordered_multimap<Id_t, std::shared_ptr<T> >;

    /**
     * \description Vector for the items from the type system
     */
    template < typename T>
    using Vec_t = std::vector<std::shared_ptr<T> >;

    /**
     *  \description List of names of inner types with and without the number of them
     */
    using InnerItemsCounterList_t = std::vector<std::pair<std::string, std::uint32_t>>;

    /**
     * \description List of the sets within the system
     */
    using SetsList_t = std::vector<std::string>;

    /**
     * \description Type of the path to describe the location of a parameter for configuration
     */
    using NamePath_t = std::vector<std::string>;
    static const NamePath_t INVALID_PATH = {"NONE"};

    struct OneSet {

        OneSet(std::string&& str) : m_name(std::forward<std::string>(str)) {
        }
        std::string m_name;
    };

    struct TwoSets {

        TwoSets(std::string&& str1, std::string&& str2)
        : m_name1(std::forward<std::string>(str1))
        , m_name2(std::forward<std::string>(str2)) {
        }
        std::string m_name1;
        std::string m_name2;
    };

    struct ThreeSets {

        ThreeSets(std::string&& str1, std::string&& str2, std::string&& str3)
        : m_name1(std::forward<std::string>(str1))
        , m_name2(std::forward<std::string>(str2))
        , m_name3(std::forward<std::string>(str3)) {
        }
        std::string m_name1;
        std::string m_name2;
        std::string m_name3;
    };

} // namespace gnsm

namespace std {

    template<> struct less<gnsm::DoubleId_t> {

        bool operator()(gnsm::DoubleId_t const& lhs, gnsm::DoubleId_t const& rhs) const noexcept {
            return (lhs.first != rhs.first ? (lhs.first <= rhs.first) : (lhs.second <= rhs.second));
        }
    };

    template<> struct equal_to<gnsm::DoubleId_t> {

        bool operator()(gnsm::DoubleId_t const& lhs, gnsm::DoubleId_t const& rhs) const noexcept {
            return (lhs.first == rhs.first && lhs.second == rhs.second);
        }
    };

    template<> struct hash<gnsm::DoubleId_t> {

        std::size_t operator()(gnsm::DoubleId_t const& di) const noexcept {
            return (std::hash<gnsm::Id_t>()(di.first) ^ std::hash<gnsm::Id_t>()(di.second));
        }
    };

    inline std::ostream& operator<<(std::ostream& os, gnsm::NamePath_t const& path) {
        auto l_ = path.size();
        if (l_ > 0) {
            os << path.at(0);
        }
        for (auto i = 1u; i < l_; ++i) {
            os << "::" << path.at(i);
            ;
        }
        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, gnsm::DoubleId_t const& did) {
        os << did.first << "-" << did.second;
        return os;
    }



}
#endif /* COREUTILS_H */

