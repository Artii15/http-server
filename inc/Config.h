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
        static Config& instance();
        void load(const std::string& path, const std::string& outerKey);
        const std::string& get(const std::string& outerKey, const std::string& innerKey);
};

#endif
