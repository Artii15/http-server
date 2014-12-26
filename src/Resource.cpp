#include "Resource.h"
#include "HttpException.h"

using namespace std;

Resource::Resource(const string& path) {
    this->path = path;
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

}

Resource::~Resource() {
    if(file.is_open()) {
        file.close();
    }
}
