#include "HttpHeaderReader.h"
#include <unistd.h>
#include <ctype.h>
#include <strings.h>

using namespace std;

HttpHeaderReader::HttpHeaderReader(const unsigned int bufSize) {
    this->bufSize = bufSize;
}

void HttpHeaderReader::readHeader(const int sck) {
    processedLine = "";
    processedHeader.clear();

    buffer = new char[bufSize]();
    do {
        ssize_t bytesReceived = read(sck, buffer, bufSize);
        processBuffer();
        bzero(buffer, bytesReceived);
    } while(!headerReaded());
    delete [] buffer;

    mergeMultipleLinedHeaders();
    mapHeader();
    linesBuffer.clear();
}

void HttpHeaderReader::processBuffer() {
    unsigned i = 0;
    while(i < bufSize && buffer[i] != '\0') {
        processedLine += buffer[i];
        if(buffer[i] == '\n') {
            linesBuffer.push_back(processedLine);
            processedLine = "";
        }
        i++;
    }
}

bool HttpHeaderReader::headerReaded() {
    if(linesBuffer.empty()) {
        return false;
    }

    string *lastLine = &(linesBuffer.back());
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
    if(linesBuffer.empty()) {
        return;
    }

    list<string>::iterator lineIt = linesBuffer.begin();

    mapFirstLine(*lineIt);
    for(lineIt++; lineIt != linesBuffer.end(); lineIt++) {
        mapAttributeLine(*lineIt);
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
}

void HttpHeaderReader::mapAttributeLine(const string &line) {
    unsigned int i = 0;
    unsigned int lineLength = line.length();

    string key = "";
    while(i < lineLength && line[i] != ':' && !isspace(line[i])) {
        key += line[i++];
    }
    i++;

    while(i < lineLength && isspace(line[i])) {
        i++;
    }
    
    string value = "";
    while(i < lineLength && line[i] != '\r' && line[i] != '\n') {
        value += line[i++];
    }

    if(!key.empty() && !value.empty()) {
        processedHeader[key] = value;
    }
}

void HttpHeaderReader::mergeMultipleLinedHeaders() {
    if(linesBuffer.empty()) {
        return;
    }

    list<string>::iterator linesIt = linesBuffer.begin();
    linesIt++;
    
    while(linesIt != linesBuffer.end()) {
        string &line = *linesIt;
        if(line[0] == ' ' || line[0] == '\t') {
            linesIt--;

            string &continuedLine = *linesIt;
            ssize_t endLinePos = getHeaderEndPos(continuedLine);

            if(endLinePos < 0) {
                continuedLine += line;
            }
            else {
                continuedLine = continuedLine.substr(0, endLinePos) + line;
            }

            linesIt++;
            linesIt = linesBuffer.erase(linesIt);
        }
        else {
            linesIt++;
        }
    }
}

ssize_t HttpHeaderReader::getHeaderEndPos(string &line) {
    int endLinePos = line.length() - 1;

    while(endLinePos >= 0) {
        if(line[endLinePos] == '\n') {
            break;
        }
        endLinePos--;
    }

    if(endLinePos > 0 && line[endLinePos - 1] == '\r') {
        endLinePos--;
    }

    return endLinePos;
}

string& HttpHeaderReader::get(const string &key) {
    return processedHeader[key]; 
}
