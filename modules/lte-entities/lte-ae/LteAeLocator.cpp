#include <sstream>
#include <fstream>


#include "LteAeLocator.h"
#include "Log.h"


LOG_REGISTER_MODULE ( "LteAeLocator" );

static const double Heigth_s = 25.0;

LteAeLocator::LteAeLocator ( std::string fileName )
{
    BEG; 
    Load (fileName);
    END;
}

void
LteAeLocator::operator() ( LteEnb& ae )
{
    BEG;
    assert(( ae.GetId() <= m_positions.size() ) &&
            "In DefaultUsersUpdater::Read ==> reading user id out of bounds");
    ae.SetPosition(m_positions.at(ae.GetId() - 1));
    END;
}

void
LteAeLocator::Load ( std::string fileName )
{
    BEG
    std::ifstream ifs_;
    ifs_.open(fileName, std::ifstream::in);
    std::string line_;

    assert(( !ifs_.fail() ) && "In DefaultAccElemLocator: "
            "==> file name is not correct check the rule to get the file names");

    while ( std::getline(ifs_, line_) )
    {
        float x_;
        float y_;
        std::istringstream ss_(line_);
        ss_ >> x_;
        ss_ >> y_;
        m_positions.push_back(Position{ x_, y_, Heigth_s });
    }
    ifs_.close();
    END
}