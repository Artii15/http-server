#ifndef HTTP_HEADER_READER_H
#define HTTP_HEADER_READER_H

#include <list>
#include <string>

using namespace std;

class HttpHeaderReader {
    private:
        int sck;
        char* buffer;
        unsigned int bufSize;

        string processedLine;
        list<string> headerLines; 

        void processBuffer();
        bool headerReaded();
    public:
        HttpHeaderReader(int sck, unsigned int bufSize = 1024); 
        void readHeader();

        ~HttpHeaderReader();
};

#endif
