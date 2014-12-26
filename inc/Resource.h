#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <fstream>
#include <boost/unordered_map.hpp>

class Resource {
    private:
        std::string path;
        std::string extension;
        std::fstream file;

        friend struct initializer;
        struct Initializer {
            Initializer();
        };
        static Initializer initializer;
        static boost::unordered_map<std::string, std::string> types;

        void openFile();
        void checkExtension();

    public:
        Resource(const std::string& path);
        ~Resource();
};

#endif
