#ifndef HTTP_HEADER_READER_H
#define HTTP_HEADER_READER_H

#include <list>
#include <string>

using namespace std;

class HttpHeaderReader {
    private:
        int sck;
        list<string> headerLines; 
        string currentLine;
    public:
        HttpHeaderReader(int sck); 
        void readHeader();
};

#endif
