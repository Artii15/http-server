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
    
    while(!finishedReading && (read(sck, buf, bufSize) > 0)) {
        getLines(buf);
        checkIfFinished();
        bzero(buf, bufSize);
    }
}

void HttpHeaderReader::getLines(char* buf) {
    unsigned i = 0;
    while(i < bufSize && buf[i] != '\0') {
        currentLine += buf[i];
        if(buf[i] == '\n') {
            headerLines.push_back(currentLine);
            currentLine = "";
        }
        i++;
    }
}

void HttpHeaderReader::checkIfFinished() {
    if(headerLines.empty()) {
        return;
    }
    if(!currentLine.empty()) {
        return;
    }
    string *lastLine = &(headerLines.back());
    int lastLineLength = lastLine->length();

    if(lastLineLength == 1 && lastLine->at(0) == '\n') {
        finishedReading = true;
        return;
    }
    if(lastLineLength > 1 && lastLine->at(0) == '\r' && lastLine->at(1) == '\n') {
        finishedReading = true;
    }
}
