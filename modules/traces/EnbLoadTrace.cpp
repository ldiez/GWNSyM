#include <iomanip>

#include "EnbLoadTrace.h"
#include "Log.h"

LOG_REGISTER_MODULE("EnbLoadTrace");


EnbLoadTrace::EnbLoadTrace(std::string const& path, std::string const& tail)
: m_iter(0u)
, m_macroTr(true, path, "enb-load_MACRO", tail)
, m_microTr(true, path, "enb-load_MICRO", tail)
, m_picoTr(true, path, "enb-load_PICO", tail)
{
    BEG;
    END;
}

void
EnbLoadTrace::SetIteration(std::uint32_t iter)
{
    BEG;
    m_iter = iter;
    INFO ("Iteration: ", m_iter);
    END;
}

void
EnbLoadTrace::operator()(gnsm::Vec_t<LteEnb> enbs)
{
    BEG;
    m_macroTr.SetIter(m_iter);
    m_microTr.SetIter(m_iter);
    m_picoTr.SetIter(m_iter);

    m_macroTr.Create();
    m_microTr.Create();
    m_picoTr.Create();

    for (auto& enb_ : enbs)
    {
        auto str_ = MakeStr(enb_);
        auto enbType_ = enb_->ReadConf().GetType();

        if (enbType_ == EnbType::MACRO)
        {
            m_macroTr.Write(str_);
        }
        else if (enbType_ == EnbType::MICRO)
        {
            m_microTr.Write(str_);
        }
        else if (enbType_ == EnbType::PICO)
        {
            m_picoTr.Write(str_);
        }
    }
    m_macroTr.Close();
    m_microTr.Close();
    m_picoTr.Close();
    END;
}

std::string
EnbLoadTrace::MakeStr(gnsm::Ptr_t<LteEnb> const& enb)
{
    BEG;
    std::stringstream ss_;
    for (auto& cell_ : enb->ReadCells())
    {
        ss_ << std::setw(10u) << std::setfill(' ') << cell_->GetLoad() << "\t";
    }
    ss_ << "\n";
    END;
    return ss_.str();
}