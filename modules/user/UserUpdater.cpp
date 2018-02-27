#include <fstream>
#include <iostream>
#include <cassert>
#include <sstream>
#include <iomanip>

#include "UserUpdater.h"
#include "Log.h"

LOG_REGISTER_MODULE ( "UserUpdater" );

static const double Height_s = 1.5;

UserUpdater::UserUpdater ( std::string strHead, std::string strTail, std::uint8_t w )
: m_strHead ( strHead ), m_strTail ( strTail ), m_w ( w )
{
    BEG END;
}

UserUpdater::~UserUpdater ( )
{
    BEG END;
}

void
UserUpdater::operator() ( User& user )
{
    BEG;
    DBG("Updating position of user ", user.GetId());
    assert(( user.GetId() <= m_lines.size() ) && "In DefaultUsersUpdater::Read"
            "==> reading user id out of bounds");
    double x_, y_;
    int videoIntention_;
    int internetIntention_;

    std::string line_(m_lines.at(user.GetId() - 1));

    std::istringstream ss_(line_);
    ss_ >> x_; // read x position
    ss_ >> y_; // read y position

    double litter_;
    ss_ >> litter_; // no sense parameter
    ss_ >> videoIntention_;
    ss_ >> internetIntention_;

    user.SetPosition({x_, y_, Height_s});

    
//    if ( m_ctr > 2 && m_ctr < 5  || m_ctr > 8 && m_ctr < 11) 
//    if ( m_currentIter == 1) 
//    {
//        user.SetVideoServiceIntention(0);
//        user.SetInternetServiceIntention(1);
//    }
//    else
//    {
//        user.SetVideoServiceIntention(0);
//        user.SetInternetServiceIntention(0);
//    }
    END;

}

void
UserUpdater::SetIteration ( std::uint32_t iter )
{
    BEG
    m_currentIter = iter;
    LoadFile();
    END
}

void
UserUpdater::LoadFile ( void )
{
    BEG
    std::stringstream ss_;
    ss_ << m_strHead << std::setw(m_w) << std::setfill('0') << m_currentIter
            << m_strTail;
    std::string fileName_(ss_.str());
    std::ifstream ifs_;
    ifs_.open(fileName_, std::ifstream::in);
    std::string line_;
    
    MSG_ASSERT(!ifs_.fail(), "File name [", fileName_, "] not found,  check the rule to get the "
            "file names and the path");
    //    assert(( !ifs_.fail() ) && "In DefaultUsersUpdater: "
    //            "==> file name is not correct check the rule to get the file names");
    m_lines.clear();
    while ( std::getline(ifs_, line_) )
    {
        m_lines.push_back(line_);
    }
    ifs_.close();
    END
}
