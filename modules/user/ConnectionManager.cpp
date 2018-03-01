#include "ConnectionManager.h"
#include "Log.h"

LOG_REGISTER_MODULE("ConnectionManager");

ConnectionManager::ConnectionManager()
: m_lteDev(nullptr)
, m_dlDemand(units::kbps(0.0))
, m_ulDemand(units::kbps(0.0)) {
    BEG END;
}

void
ConnectionManager::RegisterService(gnsm::Ptr_t<Service> serv) {
    BEG;
    m_services.push_back(serv);
    END;
}

void
ConnectionManager::RegisterDevice(gnsm::Ptr_t<LteUe> dev) {
    BEG;
    m_lteDev = dev;
    END;
}

void
ConnectionManager::Connect(gnsm::Ptr_t<LteUe> dev) {
    BEG;
    ConnectionState_e conn_;

    if (dev == nullptr) {
        conn_ = ConnectionState_e::Disconnected;
    } else {
        conn_ = ConnectionState_e::Connected;
    }

    for (auto& item_ : m_services) {
        item_->SetConnectionState(conn_);
    }
    END;
}

Traffic
ConnectionManager::DlDemand(void) const {
    BEG END;
    return m_dlDemand;
}

Traffic
ConnectionManager::UlDemand(void) const {
    BEG END;
    return m_ulDemand;
}

void
ConnectionManager::CallUp(void) {
    BEG;
    CalculateDemands();
    END;
}

void
ConnectionManager::CalculateDemands(void) {
    BEG;
    m_dlDemand = units::kbps(0.0);
    m_ulDemand = units::kbps(0.0);
    for (auto& item_ : m_services) {
        m_dlDemand += item_->DlDemand();
        m_ulDemand += item_->UlDemand();
    }
    END;
}



