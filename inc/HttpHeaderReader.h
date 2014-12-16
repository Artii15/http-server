#ifndef HTTP_HEADER_READER_H
#define HTTP_HEADER_READER_H

#include <list>
#include <map>
#include <string>

using namespace std;

class HttpHeaderReader {
    private:
        char* buffer;
        unsigned int bufSize;

        string processedLine;
        list<string> headerLines; 
        map<string, string> processedHeader;

        void processBuffer();
        bool headerReaded();
        void mapHeader();
        void mapFirstLine(const string &line);
        void mapAttributeLine(const string &line);
    public:
        HttpHeaderReader(const unsigned int bufSize = 1024); 
        void readHeader(const int sck);
        string& get(const string &key);

        ~HttpHeaderReader();
};

#endif
