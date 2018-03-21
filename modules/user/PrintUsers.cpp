#include "PrintUsers.h"
#include "Log.h"
#include "CoreUtils.h"
#include <iomanip>
#include <fstream>
#include <iomanip>
#include "lte-ae/LteCell.h"
#include "lte-ae/LteEnb.h"

LOG_REGISTER_MODULE("PrintUsers");

using Usr = gnsm::Ptr_t<User>;

void PrintHead(Usr u, std::ostream& os) {
    os << "USER " << u->GetId() << "\n";
}

void PrintLocation(Usr u, std::ostream& os) {
    auto pos = u->GetPosition();
    os << "== " << "Location ["
            << std::setw(10) << pos.GetX() << ", "
            << std::setw(10) << pos.GetY() << "]\n";
}

void PrintDlSensed(Usr u, std::ostream& os) {
    auto cells = u->GetLteDev()->GetOrderedCellsDl();
    os << "== " << "DOWNLINK sensed values\n";
    for (auto& c : cells) {
        os << "  eNB " << c.m_enbId << " - " << c.m_cell->GetId()
                << " [" << c.m_type << "]\tRSRP " << c.m_rsrp.GetDbm() << " dBm\n";
    }
}

void PrintUlSensed(Usr u, std::ostream& os) {
    auto cells = u->GetLteDev()->GetOrderedCellsUl();
    os << "== " << "UPLINK sensed values\n";
    for (auto& c : cells) {
        if (c.m_pl.RawVal() > 200) {
            continue;
        }
        os << "  eNB " << c.m_enbId << " - " << c.m_cell->GetId()
                << " [" << c.m_type << "]\tPL " << c.m_pl.RawVal() << " dB\n";
    }
}

void PrintDlConn(Usr u, std::ostream& os) {
    auto cells = u->GetLteDev()->GetDlConnList();
    auto info = u->GetLteDev()->GetDlConnInfo();
    os << "== " << "DOWNLINK connection \n";
    os << "  " << "#RBs = " << info.m_rbs << " for " << info.m_traffic.GetKbps() << " Kbps\n";
    for (auto& c : cells) {
        os << "  " << "eNB " << c->GetEnb()->GetId() << " " << c->GetId() << "\n";
    }
}

void PrintPrevDlConn(Usr u, std::ostream& os) {
    auto cells = u->GetLteDev()->GetPrevDlConnList();
    os << "== " << "Previous DOWNLINK connection \n";
    for (auto& c : cells) {
        os << "  " << "eNB " << c->GetEnb()->GetId() << " " << c->GetId() << "\n";
    }
}

void PrintUlConn(Usr u, std::ostream& os) {
    auto cells = u->GetLteDev()->GetUlConnList();
    auto info = u->GetLteDev()->GetUlConnInfo();
    os << "== " << "UPLINK connection \n";
    os << "  " << "#RBs = " << info.m_rbs << " with "
            << info.m_power.GetDbm()
            << " dBm and effective SINR " << info.m_sinr.SinrLog() << " dB\n";
    for (auto& c : cells) {
        os << "  " << "eNB " << c->GetEnb()->GetId() << " " << c->GetId() << "\n";
    }
}

void PrintPrevUlConn(Usr u, std::ostream& os) {
    auto cells = u->GetLteDev()->GetPrevUlConnList();
    os << "== " << "Previous UPLINK connection \n";
    for (auto& c : cells) {
        os << "  " << "eNB " << c->GetEnb()->GetId() << " " << c->GetId() << "\n";
    }
}

struct posRef {
    double x;
    double y;
    double val;
};

using rsrpMap = std::map<LteCell*, std::vector<posRef>>;

void PrintRsrpMap(gnsm::Vec_t<User> us) {
    rsrpMap m;
    for (auto& u : us) {
        auto cells = u->GetLteDev()->GetOrderedCellsDl();
        auto x = u->GetPosition().GetX();
        auto y = u->GetPosition().GetY();
        for (auto& c : cells) {
            auto item = m.find(c.m_cell.get());
            auto v = c.m_rsrp.GetDbm();
            if (item != m.end()) {
                item->second.push_back({x, y, v});
            } else {
                m.insert({c.m_cell.get(),
                    {
                        {x, y, v}
                    }});
            }
        }
    }
    auto i = 0u;
    for (auto& item : m) {
        std::ofstream fs;
        std::stringstream ss;
        auto folder = "./tests/rsrp/hv-full-sh/";
        //        auto folder = "./tests/rsrp/hv-full-nosh/";
        //        auto folder = "./tests/rsrp/h-full-nosh/";
        //        auto folder = "./tests/rsrp/h-los-nosh/";
        ss << folder << "rsrp" << std::setw(2) << std::setfill('0') << i << ".dat";
        fs.open(ss.str(), std::ios::out);
        for (auto& item2 : item.second) {
            fs << item2.x << "\t" << item2.y << "\t" << item2.val << "\n";
        }
        fs.close();
        ++i;
    }
}

void PrintRsrpDist(gnsm::Vec_t<User> us) {
    rsrpMap m;
    m.clear();
    for (auto& u : us) {
        auto cells = u->GetLteDev()->GetOrderedCellsDl();
        auto x = u->GetPosition().GetX();
        auto y = u->GetPosition().GetY();
        for (auto& c : cells) {
            auto item = m.find(c.m_cell.get());
            auto v = c.m_rsrp.GetDbm();
            if (item != m.end()) {
                item->second.push_back({x, y, v});
            } else {
                m.insert({c.m_cell.get(),
                    {
                        {x, y, v}
                    }});
            }
        }
    }
    auto folder = "./tests/rsrp/dist/";
    auto i = 0u;
    for (auto& c : m) {
        std::ofstream ofs;
        std::stringstream ss;
        ss << folder << "rsrpDist" << std::setw(2) << std::setfill('0') << i << ".dat";
        ofs.open(ss.str(), std::ios::app);
        for (auto& item : c.second) {
            ofs << item.x << "\t" << item.val << "\n";
        }
        ++i;
    }
}

void PrintSinr(gnsm::Vec_t<User> us) {
    auto folder = "./tests/sinr/";
    std::ofstream ofs;
    std::stringstream ss;
    ss << folder << "sinrLOS.dat";
    ofs.open(ss.str(), std::ios::app);
    for (auto& u : us) {
        auto snr = u->GetLteDev()->GetUlConnInfo().m_sinr.SinrLog();
        if (snr < -20) {
            continue;
        }
        ofs << (snr > 30 ? 30 : snr) << "\n";
        //        ofs << snr << "\n";
    }
    ofs.close();
}

void PrintTxPower(gnsm::Vec_t<User> us) {
    auto folder = "./tests/ulTxpower/";
    std::ofstream ofs;
    std::stringstream ss;
    //    ss << folder << "olTc_"
    //            << std::setw(2) << std::setfill('0') 
    //            << int(10*us.at(0)->GetLteDev()->GetConfiguration().GetAlpha()) << ".dat";
    ss << folder << "noTc.dat";
    ofs.open(ss.str(), std::ios::out);
    for (auto& u : us) {
        auto connCell = u->GetLteDev()->GetUlConnList().at(0);
        auto pow = u->GetLteDev()->GetUlConnInfo().m_power.GetDbm();
        auto enbPos = connCell->GetEnb()->GetPosition();
        auto uePos = u->GetPosition();
        auto pl = u->GetLteDev()->GetCellUl(connCell).m_pl;
        ofs << pow << "\t" << pl.m_val << "\t" << GetPlanarDistance(enbPos, uePos).GetM() << "\n";
    }
    ofs.close();
}

void PrintClosedLoop(gnsm::Vec_t<User> us, std::uint32_t iter) {
    auto folder = "./tests/cl/";
    std::ofstream ofs;
    std::stringstream ss;
    ss << folder << "cl_" << std::setw(4) << std::setfill('0') << iter << ".dat";
    ofs.open(ss.str(), std::ios::out);
    for (auto& u : us) {
        auto ulConn = u->GetLteDev()->GetUlConnInfo();
        ofs << ulConn.m_power.GetDbm() << "\t"
                << ulConn.m_sinr.SinrLog() << "\t"
                << ulConn.m_sinr.InterferenceDbm() << "\n";
    }
    ofs.close();
}

PrintUsers::PrintUsers(PrintType type)
: m_printType(type) {

}

void
PrintUsers::SetIteration(std::uint32_t iter)
{
    BEG;
    m_currIter = iter;
    END;
}

void
PrintUsers::operator()(gnsm::Vec_t<User> us) {
    BEG;
    switch (m_printType) {
        case PrintType::RSRPMAP:
            PrintRsrpMap(us);
            break;
        case PrintType::RSRPDIST:
            PrintRsrpDist(us);
            break;
        case PrintType::EFF_SINR:
            PrintSinr(us);
            break;
        case PrintType::UL_TXPOWER:
            PrintTxPower(us);
            break;
        case PrintType::CONSOLE_UL:
            for (auto& u : us) {
                PrintHead(u, std::cout);
                PrintUlConn(u, std::cout);
                PrintPrevUlConn(u, std::cout);
            }
            break;
        case PrintType::CLOSED_LOOP:
            PrintClosedLoop(us, m_currIter);
            break;
        case PrintType::CONSOLE:
        default:
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
    }
    END;
}
