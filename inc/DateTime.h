#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <string>
#include <time.h>

class DateTime {
    public:
        DateTime();
        DateTime(const std::string &date);
        std::string getDate(); 

    private:
        std::string knownFormats[3];
        time_t rawTime;
        struct tm *gmtTime;
        char formatedDate[80];

        void initializeFormats();
};


#endif
