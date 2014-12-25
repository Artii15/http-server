#ifndef HTTP_EXCEPTION
#define HTTP_EXCEPTION

#include <string>

class HttpException {
    public:
        HttpException(unsigned int code, const std::string &msg);
        unsigned int getCode();
        const std::string& getMessage();

    private:
        unsigned int code;
        std::string msg;
};

#endif
