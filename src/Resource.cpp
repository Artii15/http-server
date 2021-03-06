#include "../inc/Resource.h"
#include "../inc/HttpException.h"
#include "../inc/Config.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>

using namespace std;

Resource::Resource(const string& baseDir, const string& path) {
    this->baseDir = baseDir;
    this->path = path;
    this->extension = "";
    this->size = 0;

    normalizePath();
    validatePath();
    makeFullPath();
    openFile();
    checkExtension();
    checkSize();
    checkModificationDate();
}

void Resource::normalizePath() {
    unsigned int pathLen = path.length();

    // removing "/" from the beginning of resource path
    if(pathLen == 1 && path[0] != '/') {
        path = "";
    }
    else if(pathLen > 1 && path[0] == '/') {
        path = path.substr(1);
    }
}

void Resource::validatePath() {
    stringstream ss(path);
    string segment;

    // very simple checking if client is trying to get resource outside of domain
    int guard = 0;
    while(getline(ss, segment, '/')) {
        if(segment.empty()) {
            continue;
        }
        if(segment == "..") {
            guard--;
        }
        else if(segment != ".") {
            guard++;
        }
        if(guard < 0) {
            throw HttpException("403 Forbiden");
        }
    }
}

void Resource::makeFullPath() {
    if(baseDir.empty()) {
        throw HttpException("404 Not Found");
    }

    // Concatenating domain folder with given url
    fullPath = baseDir;
    if(!fullPath.empty() && fullPath[fullPath.length() - 1] != '/') {
        fullPath += '/';
    }
    
    fullPath += path;
}

void Resource::openFile() {
    file.open(fullPath.c_str(), ios::in | ios::binary);
    if(!file.good()) {
        throw HttpException("404 Not Found");
    }

    struct stat st_buf;
    stat(fullPath.c_str(), &st_buf);
    if(S_ISDIR(st_buf.st_mode)) { // Accessing folders not allowed
        file.close();
        throw HttpException("403 Forbidden");
    }
}

void Resource::checkExtension() {
    // Reading file extension (needed to guess mime type)
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

void Resource::checkModificationDate() {
    struct stat attrib;
    struct tm clock;

    stat(fullPath.c_str(), &attrib);
    gmtime_r(&(attrib.st_mtime), &clock);

    this->modificationDate = new DateTime(clock);
}

const string& Resource::getType() {
    // Type is based on extension. Types can be found in config/types
    return Config::instance().get("types", extension);
}

ssize_t Resource::getSize() {
    return size;
}

fstream& Resource::getResource() {
    return file;
}

const DateTime& Resource::getModificationDate() {
    return *modificationDate;
}

Resource::~Resource() {
    if(file.is_open()) {
        file.close();
    }
    delete modificationDate;
}
