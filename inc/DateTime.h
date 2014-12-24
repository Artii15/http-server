#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <string>
#include <time.h>

using namespace std;

class DateTime {
    public:
        DateTime();
        string getDate(); 

    private:
        time_t rawTime;
        struct tm *gmtTime;
        char formatedDate[80];
};


#endif
