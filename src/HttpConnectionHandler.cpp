#include "HttpConnectionHandler.h"
#include "Resource.h"
#include "DateTime.h"
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <stdexcept>

using namespace std;

HttpConnectionHandler::HttpConnectionHandler(int sck) 
: ConnectionHandler(sck) {
    httpMinor = 1; 
    statusLine = "";
    message = "";

    setStandardHeaders();
}

void HttpConnectionHandler::setStandardHeaders() {
    DateTime date;
    responseHeaders["Date"] = date.getDate();
    responseHeaders["Server"] = "Xperimental";
}

void HttpConnectionHandler::handleConnection() {
    try {
        readRequest();
        respond();
    }
    catch(HttpException &ex) {
        reportError(ex);
    }
    catch(exception &ex) {
        throw HttpException(500, "Internal Server Error");  
    }
}

void HttpConnectionHandler::readRequest() {
    reader.readHeader(sck);    
    verifyProtocolName();
    verifyVersionMajor();
    readVersionMinor();
}

void HttpConnectionHandler::verifyProtocolName() {
    const string &protocol = reader.get("protocol");
    if(protocol.substr(0, 4) != "HTTP") {
        throw HttpException(400, "Bad Request");
    }
}

void HttpConnectionHandler::verifyVersionMajor() {
    const string &protocol = reader.get("protocol");
    unsigned int protocolLen = protocol.length();

    unsigned int i = 5;
    string major = "";
    while(i < protocolLen && protocol[i] != '.') {
        major += protocol[i++];
    }
    i++;

    if(major != "1") {
        throw HttpException(501, "Not Implemented");
    }
}

void HttpConnectionHandler::readVersionMinor() {
    const string &protocol = reader.get("protocol");
    unsigned int protocolLen = protocol.length();

    string minor = "";
    for(unsigned int i = 7; i < protocolLen; i++) {
        minor += protocol[i];
    }
    istringstream iss(minor);
    iss >> this->httpMinor;

    if(this->httpMinor < 0) {
        throw HttpException(400, "Bad Request");
    }
}

void HttpConnectionHandler::respond() {
    /*
    prepareResource(); 
    sendHeaders();
    if(reader.get("method") == "GET") {
        sendResource();
    }*/
}

void HttpConnectionHandler::reportError(const HttpException &ex) {
    ostringstream ss;
    ss << "HTTP/1." << httpMinor << " " << ex.getCode() << " " << ex.getMessage();

    statusLine = ss.str();
    ss.str("");

    ss << "<html><head></head><body><h1>" << ex.getCode() << " " << ex.getMessage() << "</h1></body></html>";
    message = ss.str();

    ss.str("");
    ss << message.length();

    responseHeaders["Content-Length"] = ss.str();
    responseHeaders["Content-Type"] = "text/html";
    responseHeaders["Connection"] = "close";

    send();
}

void HttpConnectionHandler::send() {
    sendStatus();
    sendHeaders();
    sendSeparator();
    sendMessage();
}

void HttpConnectionHandler::sendStatus() {
    statusLine += "\r\n";
    size_t statusLen = statusLine.length();
    unsigned int sent = 0;

    for(unsigned int toSend = statusLen; toSend > 0; toSend -= sent) {
        sent = write(sck, statusLine.c_str(), statusLen);
    }
}

void HttpConnectionHandler::sendHeaders() {
    boost::unordered_map<string, string>::iterator headersIt = responseHeaders.begin();

    for(; headersIt != responseHeaders.end(); headersIt++) {
        ostringstream ss;
        ss << headersIt->first << ": " << headersIt->second << "\r\n";

        string header = ss.str();
        size_t headerLen = header.length();
        unsigned int sent = 0;

        for(unsigned int toSend = headerLen; toSend > 0; toSend -= sent) {
            sent = write(sck, header.c_str(), headerLen); 
        }
    }
}

void HttpConnectionHandler::sendSeparator() {
    char separator[3] = "\r\n";
    size_t sepLen = sizeof(separator) - 1;
    unsigned int sent = 0;

    for(unsigned int toSend = sepLen; toSend > 0; toSend -= sent) {
        sent = write(sck, separator, sepLen);
    }
}

void HttpConnectionHandler::sendMessage() {
    size_t messageLen = message.length();
    unsigned int sent = 0;

    for(unsigned int toSend = messageLen; toSend > 0; toSend -= sent) {
        sent = write(sck, message.c_str(), messageLen);
    }
}
