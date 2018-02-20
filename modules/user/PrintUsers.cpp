#include "PrintUsers.h"
#include "Log.h"
#include "CoreUtils.h"
#include <iomanip>


LOG_REGISTER_MODULE("PrintUsers");

using Usr = gnsm::Ptr_t<User>;

void PrintHead(Usr u, std::ostream& os) {
    os << "USER " << u->ReadId() << "\n";
}

void PrintLocation(Usr u, std::ostream& os) {
    auto pos = u->ReadPosition();
    os << "== " << "Location ["
            << std::setw(10) << pos.GetX() << ", "
            << std::setw(10) << pos.GetY() << "]\n";
}

void PrintDlSensed(Usr u, std::ostream& os) {
    auto cells = u->GetLteDev()->ReadOrderedCellsDl();
    os << "== " << "DOWNLINK sensed values\n";
    for (auto& c : cells) {
        os << "  eNB " << c.m_enbId << " - " << c.m_cell->ReadId()
                << " [" << c.m_type << "]\tRSRP " << c.m_rsrp.GetDbm() << " dBm\n";
    }
}

void PrintUlSensed(Usr u, std::ostream& os) {
    auto cells = u->GetLteDev()->ReadOrderedCellsUl();
    os << "== " << "UPLINK sensed values\n";
    for (auto& c : cells) {
        os << "  eNB " << c.m_enbId << " - " << c.m_cell->ReadId()
                << " [" << c.m_type << "]\tPL " << c.m_pl.RawVal() << " dB\n";
    }
}

void PrintDlConn(Usr u, std::ostream& os) {
    auto cells = u->GetLteDev()->ReadDlConnList();
    auto info = u->GetLteDev()->ReadDlConnInfo();
    os << "== " << "DOWNLINK connection \n";
    os << "  " << "#RBs = " << info.m_rbs << " for " << info.m_traffic.GetKbps() << " Kbps\n";
    for (auto& c : cells) {
        os << "  " << "eNB " << c->ReadEnbId() << " " << c->ReadId() << "\n";
    }
}

void PrintPrevDlConn(Usr u, std::ostream& os) {
    auto cells = u->GetLteDev()->ReadPrevDlConnList();
    os << "== " << "Previous DOWNLINK connection \n";
    for (auto& c : cells) {
        os << "  " << "eNB " << c->ReadEnbId() << " " << c->ReadId() << "\n";
    }
}

void PrintUlConn(Usr u, std::ostream& os) {
    auto cells = u->GetLteDev()->ReadUlConnList();
    auto info = u->GetLteDev()->ReadUlConnInfo();
    os << "== " << "UPLINK connection \n";
    os << "  " << "#RBs = " << info.m_rbs << " with " << info.m_power.GetDbm() << " dBm\n";
    for (auto& c : cells) {
        os << "  " << "eNB " << c->ReadEnbId() << " " << c->ReadId() << "\n";
    }
}

void PrintPrevUlConn(Usr u, std::ostream& os) {
    auto cells = u->GetLteDev()->ReadPrevUlConnList();
    os << "== " << "Previous UPLINK connection \n";
    for (auto& c : cells) {
        os << "  " << "eNB " << c->ReadEnbId() << " " << c->ReadId() << "\n";
    }
}

PrintUsers::PrintUsers() {
}

void
PrintUsers::operator()(gnsm::Vec_t<User> us) {
    BEG;
    for (auto& u : us) {
        PrintHead(u, std::cout);
        PrintLocation(u, std::cout);
        PrintDlSensed(u, std::cout);
        PrintDlConn(u, std::cout);
        PrintPrevDlConn(u, std::cout);
        PrintUlSensed(u, std::cout);
        PrintUlConn(u, std::cout);
        PrintPrevUlConn(u, std::cout);
    }
    END;
}
