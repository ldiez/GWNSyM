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

void
PrintHead(Usr u, std::ostream& os)
{
    os << "**************** USER " << u->GetId() << " ******************** \n";
}

void
PrintLocation(Usr u, std::ostream& os)
{
    auto pos = u->GetPosition();
    os << "== " << "Location ["
            << std::setw(10) << pos.GetX() << ", "
            << std::setw(10) << pos.GetY() << "]\n";
}

void
PrintDlSensed(Usr u, std::ostream& os)
{
    auto cells = u->GetLteDev()->GetOrderedCellsDl();
    os << "== " << "DOWNLINK sensed values\n";
    for (auto& c : cells)
    {
        os << "  eNB " << c.m_enbId << " - " << c.m_cell->GetId()
                << " [" << c.m_type << "]\tRSRP " << c.m_rsrp.GetDbm() << " dBm\n";
    }
}

void
PrintUlSensed(Usr u, std::ostream& os)
{
    auto cells = u->GetLteDev()->GetOrderedCellsUl();
    os << "== " << "UPLINK sensed values\n";
    for (auto& c : cells)
    {
        if (c.m_pl.RawVal() > 200)
        {
            continue;
        }
        os << "  eNB " << c.m_enbId << " - " << c.m_cell->GetId()
                << " [" << c.m_type << "]\tPL " << c.m_pl.RawVal() << " dB\n";
    }
}

void
PrintDlConn(Usr u, std::ostream& os)
{
    auto cells = u->GetLteDev()->GetDlConnList();
    auto info = u->GetLteDev()->GetDlConnInfo();
    os << "== " << "DOWNLINK connection \n";
    os << "  " << "#RBs = " << info.m_rbs << " for " << info.m_traffic.GetKbps() << " Kbps\n";
    for (auto& c : cells)
    {
        os << "  " << "eNB " << c->GetEnb()->GetId() << " " << c->GetId() << "\n";
    }
}

void
PrintPrevDlConn(Usr u, std::ostream& os)
{
    auto cells = u->GetLteDev()->GetPrevDlConnList();
    os << "== " << "Previous DOWNLINK connection \n";
    for (auto& c : cells)
    {
        os << "  " << "eNB " << c->GetEnb()->GetId() << " " << c->GetId() << "\n";
    }
}

void
PrintUlConn(Usr u, std::ostream& os)
{
    auto cells = u->GetLteDev()->GetUlConnList();
    auto info = u->GetLteDev()->GetUlConnInfo();
    os << "== " << "UPLINK connection \n";
    os << "  " << "#RBs = " << info.m_rbs << " with "
            << info.m_power.GetDbm()
            << " dBm ( " << info.m_power.GetMilliWatt() << " mW)\n";
    os << "  Effective SINR " << info.m_sinr.SinrLog() << " dB\n";
    os << "  " << "Interference " << info.m_sinr.InterferenceDbm() << " dB\n";
    for (auto& c : cells)
    {
        os << "  " << "eNB " << c->GetEnb()->GetId() << " " << c->GetId() << "\n";
    }
}

void
PrintPrevUlConn(Usr u, std::ostream& os)
{
    auto cells = u->GetLteDev()->GetPrevUlConnList();
    os << "== " << "Previous UPLINK connection \n";
    for (auto& c : cells)
    {
        os << "  " << "eNB " << c->GetEnb()->GetId() << " " << c->GetId() << "\n";
    }
}

void
PrintUsers::PrintRsrpMap(gnsm::Vec_t<User> us)
{
    rsrpMap m;
    for (auto& u : us)
    {
        auto cells = u->GetLteDev()->GetOrderedCellsDl();
        auto x = u->GetPosition().GetX();
        auto y = u->GetPosition().GetY();
        for (auto& c : cells)
        {
            auto item = m.find(c.m_cell.get());
            auto v = c.m_rsrp.GetDbm();
            if (item != m.end())
            {
                item->second.push_back({x, y, v});
            }
            else
            {
                m.insert({c.m_cell.get(),
                    {
                        {x, y, v}
                    }});
            }
        }
    }
    auto i = 0u;
    for (auto& item : m)
    {
        std::ofstream fs;
        std::stringstream ss;
        auto folder = "./tests/rsrp/hv-full-sh/";
        //        auto folder = "./tests/rsrp/hv-full-nosh/";
        //        auto folder = "./tests/rsrp/h-full-nosh/";
        //        auto folder = "./tests/rsrp/h-los-nosh/";
        ss << folder << "rsrp" << std::setw(2) << std::setfill('0') << i << ".dat";
        OpenFile(fs, ss);
        for (auto& item2 : item.second)
        {
            fs << item2.x << "\t" << item2.y << "\t" << item2.val << "\n";
        }
        fs.close();
        ++i;
    }
}

void
PrintUsers::PrintRsrpDist(gnsm::Vec_t<User> us)
{
    rsrpMap m;
    m.clear();
    for (auto& u : us)
    {
        auto cells = u->GetLteDev()->GetOrderedCellsUl();
        auto x = u->GetPosition().GetX();
        auto y = u->GetPosition().GetY();
        for (auto& c : cells)
        {
            auto item = m.find(c.m_cell.get());
            auto v = c.m_pl.RawVal();
            if (item != m.end())
            {
                item->second.push_back({x, y, v});
            }
            else
            {
                m.insert({c.m_cell.get(),
                    {
                        {x, y, v}
                    }});
            }
        }
    }
    auto folder = "./tests/";
    auto i = 0u;
    for (auto& c : m)
    {
        std::ofstream ofs;
        std::stringstream ss;
        ss << folder << "rsrpDist" << std::setw(2) << std::setfill('0') << i << ".dat";
        OpenFile(ofs, ss);
        for (auto& item : c.second)
        {
            if (item.val == LTE::TooHigh_s)
            {
                continue;
            }
            ofs << item.x << "\t" << item.y << "\t" << item.val << "\n";
        }
        ++i;
    }
}

void
PrintUsers::PrintSinr(gnsm::Vec_t<User> us)
{
    //    auto folder = "./tests/cl/";
    std::stringstream ss;
    GetName(us.at(0), ss, "Sinr");
    std::ofstream ofs;
    OpenFile(ofs, ss);
    for (auto& u : us)
    {
        auto snr = u->GetLteDev()->GetUlConnInfo().m_sinr.SinrLog();
        //        if (snr < -20)
        //        {
        //            continue;
        //        }
        //        ofs << (snr > 30 ? 30 : snr) << "  ";
        INFO("user", u->GetId(), " SINR = ", snr, "dB");
        ofs << snr << "  ";
    }
    ofs << "\n";
    ofs.close();
}

void
PrintUsers::PrintTxPower(gnsm::Vec_t<User> us)
{
    std::ofstream ofs;
    std::stringstream ss;
    GetName(us.at(0), ss, "Pow");
    OpenFile(ofs, ss);
    auto ctr = 0u;
    for (auto& u : us)
    {
        //        if (!isConnected(u->GetLteDev())) {
        //            continue;
        //        }
        auto pow = u->GetLteDev()->GetUlConnInfo().m_power.GetMilliWatt();
        //        auto connCell = u->GetLteDev()->GetUlConnList().at(0);
        //        auto enbPos = connCell->GetEnb()->GetPosition();
        //        auto uePos = u->GetPosition();
        //        auto pl = u->GetLteDev()->GetCellUl(connCell).m_pl;
        ofs << pow << "  "; // << pl.m_val << "\t" << GetPlanarDistance(enbPos, uePos).GetM() << "\n";
        ++ctr;
    }
    //    UINFO ("Added ", ctr, " columns")
    ofs << "\n";
    ofs.close();
}

PrintUsers::PrintUsers(PrintType type, UlcpType ulcp)
: m_printType(type)
, m_ulcpType(ulcp)
{

}

void
PrintUsers::SetIteration(std::uint32_t iter)
{
    BEG;
    m_currIter = iter;
    END;
}

void
PrintUsers::operator()(gnsm::Vec_t<User> us)
{
    BEG;
    m_nusers = us.size();
    switch (m_printType)
    {
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
        for (auto& u : us)
        {
            PrintHead(u, std::cout);
            PrintUlConn(u, std::cout);
            PrintPrevUlConn(u, std::cout);
        }
        break;
    case PrintType::CONSOLE:
    default:
        for (auto& u : us)
        {
            PrintHead(u, std::cout);
            //            PrintLocation(u, std::cout);
            //            PrintDlSensed(u, std::cout);
            //            PrintDlConn(u, std::cout);
            //            PrintPrevDlConn(u, std::cout);
            PrintUlSensed(u, std::cout);
            PrintUlConn(u, std::cout);
            //            PrintPrevUlConn(u, std::cout);
        }
    }
    END;
}

void
PrintUsers::OpenFile(std::ofstream& fs, std::stringstream const& ss)
{
    BEG;
    if (m_currIter == 1)
    {
        fs.open(ss.str(), std::ios::out);
    }
    else
    {
        fs.open(ss.str(), std::ios::app);
    }
    END;
}

void
PrintUsers::GetName(gnsm::Ptr_t<User> u, std::stringstream& ss, std::string med)
{
    BEG;
    auto sinrTh = u->GetLteDev()->GetConfiguration().UlSinrTh().RawVal();
    ss << "./tests/hom_";
    ss << std::setw(2) << std::setfill('0') << sinrTh << "_"
            << std::setw(2) << std::setfill('0') << m_nusers << "_";
    if (m_ulcpType == UlcpType::OL)
    {
        ss << "ol" << med << "_" << std::setw(2) << std::setfill('0')
                << int(10 * u->GetLteDev()->GetConfiguration().GetAlpha())
                << ".dat";
    }
    else
    {
        ss << (m_ulcpType == UlcpType::LP ? ("lp" + med) :
                m_ulcpType == UlcpType::CL ? ("cl" + med) : ("nc" + med))
                << ".dat";
    }
    END;
}