#include "Resource.h"
#include "HttpException.h"
#include "Config.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

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
        file.close();
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
    return Config::instance().get("types", extension);
}

ssize_t Resource::getSize() {
    return size;
}

fstream& Resource::getResource() {
    return file;
}

Resource::~Resource() {
    if(file.is_open()) {
        file.close();
    }
}
