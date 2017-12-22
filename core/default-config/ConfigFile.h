#ifndef CONFIGURATIONFILE_H
#define CONFIGURATIONFILE_H

#include <string>
#include <map>
#include <vector>
#include <fstream>

using namespace std;

#define MAXLINE 200

#define CONF_COMMENT '#'
#define CONF_SECTION_BEGIN '['
#define CONF_SECTION_END ']'
#define CONF_KEY_ASSIGN '='

enum
{
    SECTION,
    KEY,
    LIST,
} ;

typedef vector<string> listEntry_;
typedef map<string, listEntry_> listEntries;
typedef map<string, string> keyEntries;

typedef struct
{
    keyEntries keyEntry;
    listEntries listEntry;
} entries;

typedef map<string, entries *> section;

/**
 * \brief
 */
class ConfigFile
{
public:
    
    ConfigFile ( );
    ~ConfigFile ( );

    /**
     * \brief
     * \param fileName
     * \return 
     */
    int LoadConfig ( string const& fileName );
    
    /**
     * \brief
     * \param section
     * \param key
     * \param value
     * \return 
     */
    int getListValues ( const char *section, const char *key,
                        vector<string> &value );
    
    /**
     * \brief
     * \param section
     * \param key
     * \param value
     * \return 
     */
    int getKeyValue ( const char *section, const char *key, string &value );

    /**
     * \brief
     * \return
     */
    void flushConfig ( void );

private:
    
    /**
     * \brief
     * \param line
     * \return 
     */
    int suppressSpaces ( char *line );
    
    /**
     * \brief
     * \param line
     * \param section
     * \param key
     * \param values
     * \return 
     */
    int readLine ( const char *line, string &section, string &key,
                   vector<string> &values );
    
    section sectionEntries;
} ;

#endif // CONFIGURATIONFILE_H