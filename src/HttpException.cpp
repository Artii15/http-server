#include "../inc/HttpException.h"

using namespace std;

HttpException::HttpException(const string& msg) {
    this->msg = msg;
}

const string& HttpException::what() const{
    return msg;
}
