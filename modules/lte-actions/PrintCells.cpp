#include "PrintCells.h"
#include "Log.h"

LOG_REGISTER_MODULE("PrintCells");

void PrintLayerPosition(gnsm::Vec_t<LteEnb> layer, std::string const& head,
                        std::string const& tail)
{
    std::ofstream ofs_;
    ofs_.open((head + "Position_" + tail + ".dat"), std::ios::out);

    if (ofs_.is_open())
    {
        for (auto& item_ : layer)
        {
            auto pos_ = item_->GetPosition();
            UINFO (pos_.GetX(), "\t", pos_.GetY())
            ofs_ << pos_.GetX() << "\t" << pos_.GetY() << std::endl;
        }
        ofs_.close();
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

PrintCells::PrintCells(PrintType type, std::string const& head,
                       std::string const& tail)
: m_type(type)
, m_head(head)
, m_tail(tail)
{
    BEGEND;
}

void
PrintCells::operator()(gnsm::Vec_t<LteEnb> layer)
{
    
    BEG;
    if (m_type == PrintType::POSITION)
    {
        UINFO ("Printing ", layer.size(), " Cells");
        PrintLayerPosition(layer, m_head, m_tail);
    }
    END;
}


