#ifndef HTTP_HEADER_READER_H
#define HTTP_HEADER_READER_H

#include <list>
#include <boost/unordered_map.hpp>
#include <string>

class HttpHeaderReader {
    private:
        char* buffer;
        unsigned int bufSize;

        std::string processedLine;
        std::list<std::string> linesBuffer; 
        boost::unordered_map<std::string, std::string> processedHeader;

        void processBuffer();
        bool headerReaded();
        void mapHeader();
        void mapFirstLine(const std::string &line);
        void mapAttributeLine(const std::string &line);
    public:
        HttpHeaderReader(const unsigned int bufSize = 1024); 
        void readHeader(const int sck);
        std::string& get(const std::string &key);
};

#endif
