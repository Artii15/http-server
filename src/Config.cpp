#include "Config.h"
#include <fstream>
#include <stdexcept>

using namespace std;

Config& Config::factory() {
    static Config instance;
    
    return instance;
}

Config::Config() {
     
}

void Config::load(const string& path, const string& outerKey) {
    ifstream file(path.c_str());  
    if(!file.good()) {
        throw runtime_error("Couldn't open " + path);
    }
    
    string line;
    while(getline(file, line)) {
        size_t lineLen = line.length();
        string innerKey;
        string value;

        unsigned int i = 0;
        while(i < lineLen && line[i] != '=') {
            innerKey += line[i++];
        }
        i++;

        while(i < lineLen && line[i] != '\n') {
            value += line[i++];
        }

        settings[outerKey][innerKey] = value;
    }

    file.close();
}

const string& Config::get(const string& outerKey, const string& innerKey) {
    return settings[outerKey][innerKey];
}
