#include "HttpHeaderReader.h"
#include <unistd.h>
#include <strings.h>
#include <iostream>

using namespace std;

HttpHeaderReader::HttpHeaderReader(int sck, unsigned int bufSize) {
    this->sck = sck;
    this->bufSize = bufSize;
    buffer = new char[bufSize]();

    processedLine = "";
    finishedReading = true;
}

void HttpHeaderReader::readHeader() {
    finishedReading = false;
    
    while(!finishedReading && (read(sck, buffer, bufSize) > 0)) {
        processBuffer();
        checkIfFinished();
        bzero(buffer, bufSize);
    }
}

void HttpHeaderReader::processBuffer() {
    unsigned i = 0;
    while(i < bufSize && buffer[i] != '\0') {
        processedLine += buffer[i];
        if(buffer[i] == '\n') {
            headerLines.push_back(processedLine);
            processedLine = "";
        }
        i++;
    }
}

void HttpHeaderReader::checkIfFinished() {
    if(headerLines.empty()) {
        return;
    }
    if(!processedLine.empty()) {
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

HttpHeaderReader::~HttpHeaderReader() {
    delete buffer; 
}
