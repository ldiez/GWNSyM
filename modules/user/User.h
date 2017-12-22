#ifndef USER_H
#define	USER_H

#include "Wrapper.h"
#include "CoreUtils.h"
#include "UserConf.h"
#include "Position.h"

#include "lte-user/LteUe.h"
#include "ConnectionManager.h"

class User
{
public:
    explicit User ( gnsm::Id_t id );
    ~User ( );
    
    /**
     * \brief From the system
     */
    void SetConfiguration ( UserConf const& conf );
    void Aggregate ( std::string const& name, gnsm::ts::Wrapper_t o );

    /**
     * \brief Generic interface
     */
    void SetPosition ( Position pos );
    Position const& ReadPosition ( void ) const;
    gnsm::Id_t ReadId ( void ) const;
    
    /**
     * \brief Provide access to the LTE UE of the user
     * \return <-- Pointer to the device
     */
    gnsm::Ptr_t<LteUe> GetLteDev ( void );
    
    /**
     * \brief Provide access to the connection manager (just an interface)
     * \return <-- Connection manager
     */
    ConnectionManager* const GetConnectionManager ( void );

    /**
     * \brief Provide reading access to the configuration
     * \return <-- Configuration
     */
    UserConf const& ReadConf ( void ) const;

    /**
     * \brief Clear the user state so that it gets ready for the new iteration,
     * it also affect the inner classes.
     * This clears the state of the network devices and the services 
     * \return --
     */
    void CallUp ( void );
    
    /**
     * \return True if the user requires a connection false otherwise
     */
    bool IsActive ( void );
    
private:
    User () = delete;
    ConnectionManager m_connMan;
    gnsm::Ptr_t<LteUe> m_lteUe;
    const gnsm::Id_t m_id;
    Position m_pos;
    const UserConf* m_conf;

};

#endif	/* USER_H */