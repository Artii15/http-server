#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <fstream>

class Resource {
    private:
        std::string path;
        std::string type;
        std::string extension;
        std::fstream file;
    public:
        Resource(const std::string& path);
        void openFile();
        void checkExtension();
        void determineType();
        ~Resource();
};

#endif
