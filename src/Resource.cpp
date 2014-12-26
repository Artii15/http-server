#include "Resource.h"
#include "HttpException.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
    this->path = path.substr(1);
    this->extension = "";
    this->size = -1;

    openFile();
    checkExtension();
    checkSize();
}

void Resource::openFile() {
    file.open(path.c_str(), ios::in | ios::binary);
    if(!file.good()) {
        throw HttpException(404, "Not Found");
    }

    struct stat st_buf;
    stat(path.c_str(), &st_buf);
    if(S_ISDIR(st_buf.st_mode)) {
        throw HttpException(403, "Forbidden");
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

const string& Resource::getType() {
    return Resource::types[extension];
}

ssize_t Resource::getSize() {
    return size;
}

Resource::~Resource() {
    if(file.is_open()) {
        file.close();
    }
}
