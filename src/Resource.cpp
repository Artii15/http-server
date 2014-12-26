#include "Resource.h"
#include "HttpException.h"

using namespace std;

boost::unordered_map<string, string> Resource::types;

Resource::Initializer::Initializer() {
    Resource::types["html"] = "text/html"; 
    Resource::types["css"] = "text/css"; 
    Resource::types["js"] = "application/javascript"; 
    Resource::types["jpg"] = "image/jpeg"; 
    Resource::types["txt"] = "text/plain"; 
}

Resource::Resource(const string& path) {
    this->path = path;
    this->extension = "";

    openFile();
    checkExtension();
    checkSize();
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

void Resource::checkSize() {
    file.seekg(0, ios::end);
    size = file.tellg();
    file.seekg(0, ios::beg);
}

const string& Resource::getResourceType() {
    return Resource::types[extension];
}

Resource::~Resource() {
    if(file.is_open()) {
        file.close();
    }
}
