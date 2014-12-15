#include "HttpHeaderReader.h"
#include <unistd.h>
#include <strings.h>
#include <iostream>

using namespace std;

HttpHeaderReader::HttpHeaderReader(int sck, unsigned bufSize) {
    this->sck = sck;
    this->bufSize = bufSize;
    currentLine = "";
    finishedReading = true;
}

void HttpHeaderReader::readHeader() {
    char buf[bufSize];
    bzero(buf, bufSize);

    finishedReading = false;
    
    while((read(sck, buf, bufSize) > 0) && !finishedReading) {
        getLines(buf);
        bzero(buf, bufSize);
    }
}

void HttpHeaderReader::getLines(char* buf) {
    unsigned i = 0;
    while(i < bufSize && buf[i] != '\0') {
        currentLine += buf[i];
        if(buf[i] == '\n') {
            headerLines.push_back(currentLine);
            cout << currentLine;
            currentLine = "";
        }
        i++;
    }
}
