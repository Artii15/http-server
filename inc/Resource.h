#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <fstream>

class Resource {
    private:
        std::string path;
        std::string fileType;
        std::fstream file;
    public:
        Resource(const std::string& path);
        ~Resource();
};

#endif
