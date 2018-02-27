#include "LteEnb.h"
#include "Log.h"
#include "LteCell.h"

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
        item_->SetAzimut((m_conf->GetSectorizationAngle() * i_) - 30);
        item_->SetEnb(this);
        ++ i_;
    }
    END;
}

gnsm::Id_t
LteEnb::GetId ( void ) const
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
LteEnb::GetPosition ( void ) const
{
    BEG END;
    return m_pos;
}

LteEnbConf const& 
LteEnb::GetConfiguration ( void ) const
{
    BEG END;
    return *m_conf;
}

gnsm::Vec_t<LteCell> const&
LteEnb::GetCells ( void ) const
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
        if (item_->GetId() == sectorId)
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