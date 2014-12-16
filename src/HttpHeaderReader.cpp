#include "HttpHeaderReader.h"
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <iostream>

using namespace std;

HttpHeaderReader::HttpHeaderReader(const unsigned int bufSize) {
    this->bufSize = bufSize;
    buffer = new char[bufSize];
}

void HttpHeaderReader::readHeader(const int sck) {
    bzero(buffer, bufSize);
    processedLine = "";
    headerLines.clear();
    processedHeader.clear();

    do {
        ssize_t bytesReceived = read(sck, buffer, bufSize);
        processBuffer();
        bzero(buffer, bytesReceived);
    } while(!headerReaded());
    
    mapHeader();
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

    string *lastLine = &(headerLines.back());
    int lastLineLength = lastLine->length();

    if(lastLineLength > 0 && lastLine->at(0) == '\n') {
        return true;
    }
    if(lastLineLength > 1 && lastLine->at(0) == '\r' && lastLine->at(1) == '\n') {
        return true;
    }
    return false;
}

void HttpHeaderReader::mapHeader() {
    if(headerLines.empty()) {
        return;
    }

    list<string>::iterator lineIt = headerLines.begin();
    mapFirstLine(*lineIt);

    for(; lineIt != headerLines.end(); lineIt++) {
        cout << *lineIt;
    }
}

void HttpHeaderReader::mapFirstLine(const string &line) {
    unsigned int i = 0; 
    unsigned int lineLength = line.length();    

    processedHeader["method"] = "";
    while(i < lineLength && !isspace(line[i])) {
        processedHeader["method"] += line[i++];
    }
    i++;
    
    processedHeader["route"] = "";
    while(i < lineLength && !isspace(line[i])) {
        processedHeader["route"] += line[i++];
    }
    i++;

    processedHeader["protocol"] = "";
    while(i < lineLength && !isspace(line[i])) {
        processedHeader["protocol"] += line[i++];
    }

    cout << processedHeader["method"] << endl;
    cout << processedHeader["route"] << endl;
    cout << processedHeader["protocol"] << endl;
}

HttpHeaderReader::~HttpHeaderReader() {
    delete buffer; 
}
