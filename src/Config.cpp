#include "Config.h"

using namespace std;

Config& Config::factory() {
    static Config instance;
    
    return instance;
}

Config::Config() {

}

void Config::load(const char* path) {
    
}

const string& Config::get(const string& key) {
    return settings[key];
}
