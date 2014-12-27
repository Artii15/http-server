#ifndef CONFIG_H
#define CONFIG_H

#include <boost/unordered_map.hpp>
#include <string>

class Config {
    
    private:
        boost::unordered_map<std::string, 
                             boost::unordered_map<std::string, std::string> > settings;
        Config();

    public:
        static Config& factory();
        void load(const char* path, const std::string& key);
        const std::string& get(const std::string& outerKey, const std::string& innerKey);
};

#endif
