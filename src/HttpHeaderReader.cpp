#include "HttpHeaderReader.h"

HttpHeaderReader::HttpHeaderReader(int sck) {
    this->sck = sck;
    currentLine = "";
}

void HttpHeaderReader::readHeader() {

}
