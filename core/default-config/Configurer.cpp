#include <cassert> // for assert
#include <algorithm> // for std::transform

#include "Configurer.h"
#include "Log.h"

LOG_REGISTER_MODULE("Configurer")

namespace gnsm {

    Configurer::Configurer(std::string const& path) {
        BEG
        m_configFile.flushConfig();
        auto check_ = m_configFile.LoadConfig(path);
        MSG_ASSERT(check_ >= 0, "Configuration file [" + path + "] not found");
        END
    }

    Configurer::~Configurer() {
        BEG
        m_configFile.flushConfig();
        END
    }

    int
    Configurer::GetParamInt(NamePath_t namePath) {
        BEG
        assert(namePath.size() == 2 &&
                "In Configurer::GetParamInt ==> Bad manePathSize");
        
        

        std::string value_;
        
        if (m_configFile.getKeyValue(const_cast<char*> (namePath[0].c_str())
                , const_cast<char*> (namePath[1].c_str()), value_)) {
            MSG_ASSERT(false, "BAD param. reading: ", namePath[0].c_str(), " :: ", namePath[1].c_str());
        }
        END
        return std::stoi(value_);
    }

    double
    Configurer::GetParamFloat(NamePath_t namePath) {

        BEG
        assert(namePath.size() == 2 &&
                "In Configurer::GetParamFloat ==> Bad manePathSize");

        std::string value_;
        if (m_configFile.getKeyValue(const_cast<char*> (namePath[0].c_str())
                , const_cast<char*> (namePath[1].c_str())
                , value_)) {
            for (auto& item_ : namePath) {
                std::cout << item_ << " -- ";
            }
            ERROR("BAD param. reading: ", namePath[0], " :: ", namePath[1]);
            assert(false);
        }
        END
        return std::stof(value_);
    }

    std::string
    Configurer::GetParamStr(NamePath_t namePath) {
        BEG
        assert(namePath.size() == 2 &&
                "In Configurer::GetParamStr ==> Bad manePathSize");

        std::string value_;
        if (m_configFile.getKeyValue(const_cast<char*> (namePath[0].c_str())
                , const_cast<char*> (namePath[1].c_str())
                , value_)) {
            ERROR("BAD param. reading: %s :: %s", namePath[0].c_str(), namePath[1].c_str());
            assert(false);
        }
        END
        return value_;
    }

    int
    Configurer::GetParamInt(NamePath_t namePath, int defVal) {
        BEG
        assert(namePath.size() == 2 &&
                "In Configurer::GetParamInt ==> Bad manePathSize");

        std::string value_;
        if (m_configFile.getKeyValue(const_cast<char*> (namePath[0].c_str())
                , const_cast<char*> (namePath[1].c_str())
                , value_)) {
            return defVal;
        }
        END
        return std::stoi(value_);
    }

    double
    Configurer::GetParamFloat(NamePath_t namePath, double defVal) {
        BEG
        assert(namePath.size() == 2 &&
                "In Configurer::GetParamFloat ==> Bad manePathSize");

        std::string value_;
        if (m_configFile.getKeyValue(const_cast<char*> (namePath[0].c_str())
                , const_cast<char*> (namePath[1].c_str())
                , value_)) {
            return defVal;
        }
        END
        return std::stof(value_);
    }

    std::string
    Configurer::GetParamStr(NamePath_t namePath, std::string defVal) {
        BEG
        assert(namePath.size() == 2 &&
                "In Configurer::GetParamStr ==> Bad manePathSize");

        std::string value_;
        if (m_configFile.getKeyValue(const_cast<char*> (namePath[0].c_str())
                , const_cast<char*> (namePath[1].c_str())
                , value_)) {
            END
            return defVal;
        }
        END
        return value_;
    }

    std::vector<int>
    Configurer::GetListInt(NamePath_t namePath) {
        BEG
        assert(namePath.size() == 2 &&
                "In Configurer::GetParamListInt ==> Bad manePathSize");

        std::vector<std::string> value_;
        if (m_configFile.getListValues(const_cast<char*> (namePath[0].c_str()), const_cast<char*> (namePath[1].c_str()), value_)) {
            ERROR("BAD param. reading: ", namePath[0], " :: ", namePath[1]);
            assert(false);
        }
        std::vector<int> ret_;
        ret_.resize(value_.size());
        std::transform(value_.begin(), value_.end(), ret_.begin(), [](const std::string & elem_) {
            return std::stoi(elem_);
        });
        END
        return ret_;
    }

    std::vector<double>
    Configurer::GetListFloat(NamePath_t namePath) {
        BEG
        assert(namePath.size() == 2 &&
                "In Configurer::GetParamListFloat ==> Bad manePathSize");

        std::vector<std::string> value_;
        if (m_configFile.getListValues(const_cast<char*> (namePath[0].c_str()), const_cast<char*> (namePath[1].c_str()), value_)) {
            ERROR("BAD param. reading: ", namePath[0], " :: ", namePath[1]);
            assert(false);
        }
        std::vector<double> ret_;
        ret_.resize(value_.size());
        std::transform(value_.begin(), value_.end(), ret_.begin(), [](const std::string & elem_) {
            return std::stof(elem_);
        });
        END
        return ret_;
    }

    std::vector<std::string>
    Configurer::GetListStr(NamePath_t namePath) {
        BEG
        assert(namePath.size() == 2 &&
                "In Configurer::GetParamListStr ==> Bad manePathSize");

        std::vector<std::string> value_;
        if (m_configFile.getListValues(const_cast<char*> (namePath[0].c_str()), const_cast<char*> (namePath[1].c_str()), value_)) {
            ERROR("BAD param. reading: ", namePath[0], " :: ", namePath[1]);
            assert(false);
        }
        END
        return value_;
    }

    std::vector<int>
    Configurer::GetListInt(NamePath_t namePath, std::vector<int> defVal) {
        BEG
        assert(namePath.size() == 2 &&
                "In Configurer::GetParamListInt ==> Bad manePathSize");

        std::vector<std::string> value_;
        if (m_configFile.getListValues(const_cast<char*> (namePath[0].c_str()), const_cast<char*> (namePath[1].c_str()), value_)) {
            return defVal;
        }
        std::vector<int> ret_;
        ret_.resize(value_.size());
        std::transform(value_.begin(), value_.end(), ret_.begin(), [](const std::string & elem_) {
            return std::stoi(elem_);
        });
        END
        return ret_;
    }

    std::vector<double>
    Configurer::GetListFloat(NamePath_t namePath, std::vector<double> defVal) {
        BEG
        assert(namePath.size() == 2 &&
                "In Configurer::GetParamListFloat ==> Bad manePathSize");

        std::vector<std::string> value_;
        if (m_configFile.getListValues(const_cast<char*> (namePath[0].c_str()), const_cast<char*> (namePath[1].c_str()), value_)) {
            return defVal;
        }
        std::vector<double> ret_;
        ret_.resize(value_.size());
        std::transform(value_.begin(), value_.end(), ret_.begin(), [](const std::string & elem_) {
            return std::stof(elem_);
        });
        END
        return ret_;
    }

    std::vector<std::string>
    Configurer::GetListStr(NamePath_t namePath, std::vector<std::string> defVal) {
        BEG
        assert(namePath.size() == 2 &&
                "In Configurer::GetParamListStr ==> Bad manePathSize");

        std::vector<std::string> value_;
        if (m_configFile.getListValues(const_cast<char*> (namePath[0].c_str()), const_cast<char*> (namePath[1].c_str()), value_)) {
            return defVal;
        }
        END
        return value_;
    }

} // namespace gnsm