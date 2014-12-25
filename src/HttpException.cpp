#include "HttpException.h"

using namespace std;

HttpException::HttpException(unsigned int code, const string &msg) {
    this->msg = msg;
    this->code = code;
}

unsigned int HttpException::getCode() {
    return code;
}

const string& HttpException::getMessage() {
    return msg;
}


