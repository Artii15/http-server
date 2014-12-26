#include "Resource.h"
#include "HttpException.h"

using namespace std;

Resource::Resource(const string& path) {
    file.open(path.c_str(), ios::in | ios::binary);
}

Resource::~Resource() {
    if(file.is_open()) {
        file.close();
    }
}
