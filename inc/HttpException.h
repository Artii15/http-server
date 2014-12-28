#ifndef HTTP_EXCEPTION
#define HTTP_EXCEPTION

#include <string>

class HttpException {
    private:
        std::string msg;
    public:
        HttpException(const std::string &msg);
        const std::string& what() const;
};

#endif
