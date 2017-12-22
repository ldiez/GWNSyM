#include <cstring>

#include "ConfigFile.h"
#include "Log.h"

LOG_REGISTER_MODULE ("ConfigFile");

ConfigFile::ConfigFile ( )
{
    BEG END
}

ConfigFile::~ConfigFile ( )
{

    BEG END
}

int
ConfigFile::suppressSpaces ( char *line )
{
    int len;
    int i, j;

    BEG

    i = 0;
    j = 0;

    len = strlen (line);
    if (len > 0)
    {
        while (isspace (line[i]) || line[i] == '\t')
        {
            i++;
        }
        while (i <= len)
        {
            line[j++] = line[i++];
        }

        i = strlen (line) - 1;
        while (i > 0  && (isspace (line[i]) || line[i] == '\t'))
        {
            line[i--] = '\0';
        }
        len = strlen (line);
    }

    END
    return len;
}

int
ConfigFile::readLine ( const char *line, string &section, string &key, vector<string> &values )
{
    int len;
    int i = 0, j = 0;
    string value;
    string key_;
    int separator;
    int ret;

    BEG

    section = key = "";
    len = strlen (line);

    if (line[0] == CONF_SECTION_BEGIN && line[len - 1] == CONF_SECTION_END)
    {
        section = line + 1;
        section.erase (section.size () - 1, 1);
        ret = SECTION;
    }
    else
    {
        key_ = line;
        separator = key_.find (CONF_KEY_ASSIGN, 0);
        if (separator != -1)
        {       // Pair key_value
            key = key_.substr (0, separator);
            value = key_.substr (separator + 1, key_.size () - separator);
            values.push_back (value);
            ret = KEY;
        }
        else
        {                // List
            while (i < len)
            {
                while (!isspace (line[i]) && line[i] != '\t' && i < len)
                {
                    i++;
                }
                value = (line + j);
                if (i < len)
                {
                    value.erase (i - j, value.size ()-(i - j));
                }
                if (j == 0)
                {
                    key = value;
                }
                else
                {
                    values.push_back (value);
                }
                value.clear ();
                j = ++i;
            }
            //key = values[0];
            //values[0].clear();
            //values.erase(values.begin(), values.begin() + 1);
            ret = LIST;
        }
    }

    END
    return ret;
}

int
ConfigFile::getListValues ( const char *section, const char *key, vector<string> &value )
{
    string section_ = section;
    string key_ = key;
    entries *entry;

    BEG

    if (sectionEntries.count (section_))
    {
        entry = sectionEntries[section_];
        if (entry->listEntry.count (key_))
        {
            value = entry->listEntry[key_];
        }
        else
        {
            return -1;
        }
    }
    else
    {

        return -1;
    }

    END
    return 0;
}

int
ConfigFile::getKeyValue ( const char *section, const char *key, string &value )
{
    string section_ = section;
    string key_ = key;
    entries *entry;

    BEG

    if (sectionEntries.count (section_))
    {
        entry = sectionEntries[section_];
        if (entry->keyEntry.count (key_))
        {
            value = entry->keyEntry[key_];
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    END
    return 0;
}

int
ConfigFile::LoadConfig ( string const&  fileName )
{
    fstream confFile;
    vector<string> lines;
    vector<string> values;
    char line[MAXLINE];
    unsigned int i;
    string sectionName, key;
    entries *sectionEntry = NULL;
    section::iterator iter1;
    keyEntries::iterator iter2;
    listEntries::iterator iter3;

    BEG

    confFile.open (fileName.c_str (), ios::in);
    if (!confFile)
    {
        return (-1);
    }

    while (confFile.getline (line, MAXLINE))
    {
        if (suppressSpaces (line) > 0 && line[0] != CONF_COMMENT)
        {
            lines.push_back (line);
        }
    }


    for (i = 0; i < lines.size (); i++)
    {
        switch (readLine ((char *) lines[i].c_str (), sectionName, key, values))
        {
            case SECTION:
                sectionEntry = new entries;
                sectionEntries[sectionName] = sectionEntry;
                values.clear ();
                break;
            case KEY:
                sectionEntry->keyEntry[key] = values[0];
                values.clear ();
                break;
            case LIST:
                sectionEntry->listEntry[key] = values;
                values.clear ();
                break;
        }
    }

    lines.clear ();
    confFile.clear ();

    END
    return 0;
}

void
ConfigFile::flushConfig ( void )
{
    section::iterator iter1;
    listEntries::iterator iter3;
    int i;
    BEG 
    for (iter1 = sectionEntries.begin () ; iter1 != sectionEntries.end () ; ++iter1)
    {
        (iter1->second)->keyEntry.clear ();
        for (iter3 = iter1->second->listEntry.begin () ; iter3 != iter1->second->listEntry.end () ; ++iter3)
        {
            for (i = 0 ; i < (signed) (iter3->second).size () ; i++)
            {
                (iter3->second)[i].clear ();
            }
            (iter3->second).clear ();
        }

        (iter1->second)->listEntry.clear ();
        delete(iter1->second);
    }
    sectionEntries.clear ();
    END
}