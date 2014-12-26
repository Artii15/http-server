#include "Resource.h"
#include "HttpException.h"

using namespace std;

boost::unordered_map<string, string> Resource::types;

Resource::Initializer::Initializer() {
    Resource::types["html"] = "text/html"; 
    Resource::types["css"] = "text/css"; 
    Resource::types["js"] = "application/javascript"; 
    Resource::types["jpg"] = "image/jpeg"; 
}

Resource::Resource(const string& path) {
    this->path = path;
    this->type = "";
    this->extension = "";

    openFile();
    checkExtension();
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

Resource::~Resource() {
    if(file.is_open()) {
        file.close();
    }
}
