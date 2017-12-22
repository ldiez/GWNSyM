#ifndef TRACE_H
#define TRACE_H

#include <string>
#include <fstream>

class Trace
{
public:

    Trace(bool iter, std::string const& path, std::string const& name);
    Trace(bool iter, std::string const& path, std::string const& name, std::string const& tail);
    
    ~Trace ();


    /**
     * \brief Set iteration format of the iteration information. By default it takes 4 digits and
     * 0 as to fill
     * \param res --> Resolution (number of digits)
     * \param fill --> 
     * \return --
     */
    void SetIterFormat(std::uint8_t res, char fill = '0');

    /**
     * \brief Set iteration number. 
     * \param iter --> 
     * \return --
     */
    void SetIter(std::uint32_t iter);
    
    /**
     * \brief Set file extension
     * \return --
     */
    void SetExt(std::string const& ext);

    /**
     * \brief It creates a trace file with the current configuration
     * \param mode --> Open mode
     * \return --
     */
    void Create(std::ios::openmode mode = std::ios::out);

    /**
     * \brief Write in the trace file
     * \param str --> Information to write
     * \return --
     */
    void Write(std::string const& str);

    /**
     * \brief Close the current trace file
     * \return --
     */
    void Close(void);

    /**
     * \brief Give the filename with the current settings
     * \return --
     */
    std::string GetName(void);

private:
    const bool m_iterable;
    const std::string m_path;
    const std::string m_name;
    const std::string m_tail;
    std::string m_ext;

    std::uint8_t m_iterRes;
    char m_fill;
    std::uint32_t m_currIter;
    std::ofstream m_ofs;
};

#endif /* TRACE_H */

