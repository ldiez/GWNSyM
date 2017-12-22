#ifndef USERUPDATER_H
#define	USERUPDATER_H

#include "User.h"

class UserUpdater
{
public:
    UserUpdater ( std::string strHead, std::string strTail, std::uint8_t w );
    ~UserUpdater ( );
    
    void operator () ( User& user);
    
    void SetIteration ( std::uint32_t iter );
private:
    
    void LoadFile ( void );

    std::uint32_t m_currentIter;
    const std::string m_strHead;
    const std::string m_strTail;
    const std::uint8_t m_w;
    std::vector <std::string> m_lines;
    
};

#endif	/* USERUPDATER_H */

