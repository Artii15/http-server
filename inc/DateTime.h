#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <string>
#include <time.h>
#include <boost/regex.hpp>

class DateTime {
    private:
        boost::regex acceptedFormats[3];
        time_t rawTime;
        struct tm gmtTime;
        char formatedDate[80];

        void initializeFormats();

    public:
        DateTime();
        DateTime(const std::string &date);
        DateTime(const struct tm& clock);
        std::string getDate() const;
        bool operator==(const DateTime& date) const;
        bool operator>(const DateTime& date) const;
};


#endif
