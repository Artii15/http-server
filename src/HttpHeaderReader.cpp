#include "HttpHeaderReader.h"
#include <unistd.h>
#include <strings.h>
#include <iostream>

using namespace std;

HttpHeaderReader::HttpHeaderReader(unsigned int bufSize) {
    this->bufSize = bufSize;
    buffer = new char[bufSize];
}

void HttpHeaderReader::readHeader(int sck) {
    bzero(buffer, bufSize);
    processedLine = "";

    do {
        ssize_t bytesReceived = read(sck, buffer, bufSize);
        processBuffer();
        bzero(buffer, bytesReceived);
    } while(!headerReaded());
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

bool HttpHeaderReader::headerReaded() {
    if(headerLines.empty()) {
        return false;
    }
    if(!processedLine.empty()) {
        return false;
    }
    string *lastLine = &(headerLines.back());
    int lastLineLength = lastLine->length();

    if(lastLineLength == 1 && lastLine->at(0) == '\n') {
        return true;
    }
    if(lastLineLength > 1 && lastLine->at(0) == '\r' && lastLine->at(1) == '\n') {
        return true;
    }
    return false;
}

HttpHeaderReader::~HttpHeaderReader() {
    delete buffer; 
}
