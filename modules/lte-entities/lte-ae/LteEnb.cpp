#include "LteEnb.h"
#include "Log.h"

LOG_REGISTER_MODULE ( "LteEnb" )

LteEnb::LteEnb ( gnsm::Id_t id ) : m_id ( id ), m_pos ( 0.0, 0.0, 25.0 )
{
    BEG END;
}

void
LteEnb::SetConfiguration ( LteEnbConf const& conf )
{
    BEG;
    m_conf = &conf;
    END;
}

void
LteEnb::Aggregate ( std::string const& name, gnsm::ts::Wrapper_t o )
{
    BEG;
    if ( name ==  m_conf->GetCellSetName())
    {
        o.Unwrap(m_cells);
    }

    auto i_ = 0u;
    for ( auto& item_ : m_cells )
    {
        item_->SetAzimut(m_conf->GetSectorizationAngle() * i_);
        item_->SetEnbId(m_id);
        ++ i_;
    }

    END;
}

gnsm::Id_t
LteEnb::ReadId ( void ) const
{
    BEG END;
    return m_id;
}

void
LteEnb::SetPosition ( Position pos )
{
    BEG END;
    INFO ("Position at", pos);
    m_pos = pos;
}

Position const&
LteEnb::ReadPosition ( void ) const
{
    BEG END;
    return m_pos;
}

LteEnbConf const& 
LteEnb::ReadConf ( void ) const
{
    BEG END;
    return *m_conf;
}

gnsm::Vec_t<LteCell> const&
LteEnb::ReadCells ( void ) const
{
    BEG END;
    return m_cells;
}

gnsm::Ptr_t<LteCell> 
LteEnb::GetCell ( gnsm::Id_t sectorId) const
{
    BEG;
    for (auto& item_ : m_cells)
    {
        if (item_->ReadId() == sectorId)
        {
            END;
            return item_;
        }
    }
    return nullptr;
    END;
}

void 
LteEnb::CallUp ( void )
{
    BEG;
    for (auto& item_ : m_cells)
    {
        item_->CallUp();
    }
    END;
}