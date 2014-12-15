#ifndef HTTP_HEADER_READER_H
#define HTTP_HEADER_READER_H

#include <list>
#include <string>

using namespace std;

class HttpHeaderReader {
    private:
        int sck;
        unsigned bufSize;
        bool finishedReading;
        list<string> headerLines; 
        string currentLine;

        void getLines(char* buf);
        void checkIfFinished();
    public:
        HttpHeaderReader(int sck, unsigned bufSize = 1000); 
        void readHeader();
};

#endif
