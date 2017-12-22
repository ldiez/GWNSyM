#ifndef CONFIGXML_H
#define CONFIGXML_H

#include <boost/date_time.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

/**
 * \brief Just a simple wrapper for boost::property_tree_xml_parser to
 * read/write a configuration file
 */
class ConfigXml
{
    using Tree_t = boost::property_tree::ptree;
public:

    inline ConfigXml(std::string const& filename)
    {
        boost::property_tree::xml_parser::read_xml(filename, m_pt);
    }

    /**
     * \brief Read the value of a configurtaion parameter
     * \param path --> Path to the parameter
     * \return <-- Parameter value casted to \T (by default std::string)
     */
    template < typename T = std::string >
    T Get(std::string const& path) const;
    template < typename T = std::string >
    T Get(std::string const& path, T t) const;

    /**
     * \brief Read the value of an attribute of a  configuration parameter
     * \param path --> Path to the parameter
     * \param attrName --> Name of the attribute
     * \return <-- Attribute value casted to \T (by default std::string)
     */
    template < typename T = std::string >
    T GetAttr(std::string const& path, std::string const& attrName) const;

private:
    ConfigXml() = delete;
    Tree_t m_pt;
};

// implementation

template < typename T >
T ConfigXml::Get(std::string const& path) const
{
    return m_pt.get<T>(path);
}

template < typename T >
T ConfigXml::Get(std::string const& path, T t) const
{
    return m_pt.get<T>(path, t);
}

template < typename T >
T ConfigXml::GetAttr(std::string const& path, std::string const& attrName) const
{
    return m_pt.get_child(path).get<T>("<xmlattr>." + attrName);
}


#endif /* CONFIGXML_H */

