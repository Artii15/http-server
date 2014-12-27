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

void Config::load(const char* path, const string& key) {
    ifstream file(path);  

    file.close();
}

const string& Config::get(const string& outerKey, const string& innerKey) {
    return settings[outerKey][innerKey];
}
