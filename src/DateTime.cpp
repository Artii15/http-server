#include "DateTime.h"
#include <string>

using namespace std;
using namespace boost;

DateTime::DateTime() {
    rawTime = time(NULL);  
    gmtime_r(&rawTime, &gmtTime);
    strftime(formatedDate, sizeof(formatedDate), "%a, %d %b %Y %H:%M:%S GMT", &gmtTime);
}

DateTime::DateTime(const string &date) {
    initializeFormats();
}

DateTime::DateTime(const struct tm& clock) {
    gmtTime = clock;
    rawTime = mktime(&gmtTime);
    strftime(formatedDate, sizeof(formatedDate), "%a, %d %b %Y %H:%M:%S GMT", &gmtTime);
}

void DateTime::initializeFormats() {
    acceptedFormats[0] = regex("^(\\u\\l{2}),\\s(\\d{1,2})\\s(\\u\\l{2})\\s(\\d{4})\\s(\\d{1,2}):(\\d{1,2}):(\\d{1,2})\\sGMT$");
    acceptedFormats[1] = regex("^(\\u\\l+),\\s(\\d{1,2})-(\\u\\l{2})-(\\d{2})\\s(\\d{1,2}):(\\d{1,2}):(\\d{1,2})\\sGMT$");
    acceptedFormats[2] = regex("^(\\u\\l{2})\\s(\\u\\l{2})\\s(\\d{1,2})\\s(\\d{1,2}):(\\d{1,2}):(\\d{1,2})\\s(\\d{4})$");
}

string DateTime::getDate() const {
    return formatedDate;
}

bool DateTime::operator==(const DateTime& date) const {
    return this->rawTime == date.rawTime; 
}

bool DateTime::operator>(const DateTime& date) const {
    return this->rawTime > date.rawTime;
}
