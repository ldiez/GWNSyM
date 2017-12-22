#include "SaSelection.h"
#include "Log.h"

LOG_REGISTER_MODULE("SaSelection");

SaSelection::SaSelection(double backgroundLoad, double interLoad, units::dB cre)
: m_utility()
, m_sa(std::bind(&SaUtility::Cost, &m_utility, std::placeholders::_1))
, m_lt(backgroundLoad, interLoad, cre)
{
    BEG END;
}

SaSelection::~SaSelection()
{
    BEG END;
}

void
SaSelection::operator()(gnsm::Vec_t<User> users, gnsm::Vec_t<LteCell> interCells)
{
    BEG;
    Setup(users, interCells);
    Solve();
    Connect();
    END;
}

std::vector<double> 
SaSelection::GetSolution(gnsm::Vec_t<User> users, gnsm::Vec_t<LteCell> interCells)
{
    BEG;
    Setup(users, interCells);
    Solve();
    END;
    return m_results;
}

void
SaSelection::Setup(gnsm::Vec_t<User>& users, gnsm::Vec_t<LteCell>& interCells)
{
    BEG;
    m_results = std::vector<double>(users.size());
    m_tempInfo = m_lt.Translate(users, interCells);
    m_utility.SetNetState(m_tempInfo);

    for (auto& item_ : users)
    {
        auto demand_ = item_->GetConnectionManager()->DlDemand();
        m_ic.AddUnit(demand_.GetKbps());
    }
    END;
}

void
SaSelection::Solve(void)
{
    BEG;
    m_utility.Cost(m_ic);
    m_sa.SetInfo(m_ic);
    m_sa.Anneal();
    
    MSG_ASSERT (m_results.size() == m_ic.GetNumUnits(), "Size of the results and info-carrier "
            "do not match");
    for (auto i_ = 0u; i_ < m_ic.GetNumUnits(); ++i_)
    {
        m_results.at(i_) = m_ic.GetAssignment(i_);
    }
    END;
}

void
SaSelection::Connect(void)
{
    BEG;
    auto ctr_ = 0u;
    for (auto& user_ : m_tempInfo)
    {
        INFO ("Connecting user ", ctr_+1, " to ", user_.m_currCell);
        auto check_ = user_.m_currCell->AddUser(m_ic.GetAssignment(ctr_));
        ConnectUser(user_, ctr_);
        MSG_ASSERT(check_, "Cell capacity exceeded!!");
        ++ctr_;
    }
    m_results.clear();
    m_ic.Clear();
    END;
}

void 
SaSelection::ConnectUser (TranslatedInfo& ti, std::uint32_t index)
{
    BEG;
    auto dev_ = ti.m_self->GetLteDev();
    
    LteUe::DlConn dlConn_;
    
    dlConn_.m_rbs = m_ic.GetAssignment(index);
    auto kbps_ = m_ic.GetTarget(index) + m_ic.GetThputExcess(index);
    dlConn_.m_traffic = Traffic(units::kbps(kbps_));
    
    dev_->DlConnect({ti.m_currCell}, dlConn_); 
    
    ti.m_self->GetConnectionManager()->Connect(dev_);
    
    END;
}