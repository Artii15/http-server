#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <fstream>

class Resource {
    private:
        std::string baseDir;
        std::string path;
        std::string fullPath;
        std::string extension;
        std::fstream file;
        ssize_t size;

        void validatePath();
        void makeFullPath();
        void openFile();
        void checkExtension();
        void checkSize();

    public:
        Resource(const std::string& baseDir, const std::string& path);
        const std::string& getType();
        ssize_t getSize();
        std::fstream& getResource();
        ~Resource();
};

#endif
