#include <sstream>
#include <iomanip>
#include <boost/filesystem.hpp>

#include "Trace.h"
#include "Log.h"

LOG_REGISTER_MODULE("Trace");

Trace::Trace(bool iter, std::string const& path, std::string const& name)
: m_iterable(iter)
, m_path(path)
, m_name(name)
, m_tail("")
, m_ext("tr")
, m_iterRes(4u)
, m_fill('0')
, m_currIter(0u)
{
    BEG END;
}

Trace::Trace(bool iter, std::string const& path, std::string const& name, std::string const& tail)
: m_iterable(iter)
, m_path(path)
, m_name(name)
, m_tail(tail)
, m_ext("tr")
, m_iterRes(4u)
, m_fill('0')
, m_currIter(0u)
{
    BEG END;
}

Trace::~Trace ()
{
    BEG;
    if (m_ofs.is_open())
    {
        m_ofs.close();
    }
    END;
}

void
Trace::SetIterFormat(std::uint8_t res, char fill)
{
    BEG;
    m_iterRes = res;
    m_fill = fill;
    INFO("Iteration format: resolution ", std::to_string(m_iterRes), " and fill ", m_fill);
    END;
}

void
Trace::SetIter(std::uint32_t iter)
{
    BEG;
    m_currIter = iter;
    INFO("Current iteration: ", m_currIter);
    END;
}

void 
Trace::SetExt(std::string const& ext)
{
    BEG;
    m_ext = ext;
    INFO ("Current trace extension: ", m_ext);
    END;
}

void
Trace::Create(std::ios::openmode mode)
{
    BEG;
    boost::filesystem::create_directories(m_path);
    if (m_ofs.is_open())
    {
        m_ofs.close();
    }
    
    m_ofs.open(GetName(), mode);
    
    END;
}

void
Trace::Write(std::string const& str)
{
    BEG;
    if (m_ofs.is_open())
    {
        m_ofs << str;
    }
    END;
}

void
Trace::Close(void)
{
    BEG;
    if (m_ofs.is_open())
    {
        m_ofs.close();
    }
    END;
}

std::string
Trace::GetName(void)
{
    BEG;
    std::stringstream ss_;
    std::stringstream ssAux_;
    if (m_iterable)
    {
        ssAux_ << "_" << std::setw(m_iterRes) << std::setfill(m_fill) << m_currIter;
    }
    ss_ << m_path << "/" << m_name << ssAux_.str() << m_tail << "." << m_ext;
    END;
    return ss_.str();
    

}