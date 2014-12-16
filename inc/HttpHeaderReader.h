#ifndef HTTP_HEADER_READER_H
#define HTTP_HEADER_READER_H

#include <list>
#include <string>

using namespace std;

class HttpHeaderReader {
    private:
        char* buffer;
        unsigned int bufSize;

        string processedLine;
        list<string> headerLines; 

        void processBuffer();
        bool headerReaded();
    public:
        HttpHeaderReader(unsigned int bufSize = 1024); 
        void readHeader(int sck);

        ~HttpHeaderReader();
};

#endif
