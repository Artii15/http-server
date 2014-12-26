#include "Resource.h"
#include "HttpException.h"
#include <boost/unordered_map.hpp>

using namespace std;

Resource::Resource(const string& path) {
    this->path = path;
    this->type = "";
    this->extension = "";

    openFile();
    checkExtension();
    determineType();
}

void Resource::openFile() {
    file.open(path.c_str(), ios::in | ios::binary);
    if(!file.good()) {
        throw HttpException(404, "Not Found");
    }
}

void Resource::checkExtension() {
    unsigned int dotPos = path.find_last_of('.');
    if(dotPos != string::npos) {
        extension = path.substr(dotPos + 1);
    }
}

void Resource::determineType() {
    boost::unordered_map<string, string> types;

    types["html"] = "text/html";
    const string& type = types[extension];
    
    if(type.empty()) {
        this->type = "text/plain";
    }
    else {
        this->type = type;
    }
}

Resource::~Resource() {
    if(file.is_open()) {
        file.close();
    }
}
