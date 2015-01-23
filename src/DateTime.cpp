#include "DateTime.h"
#include <stdexcept>
#include <string>
#include <string.h>

using namespace std;
using namespace boost;

DateTime::DateTime() {
    // This constructor creates object representing current date
    rawTime = time(NULL);  
    gmtime_r(&rawTime, &gmtTime);
    strftime(formatedDate, sizeof(formatedDate), "%a, %d %b %Y %H:%M:%S GMT", &gmtTime);
}

DateTime::DateTime(const DateTime& date) {
    // Copy constructor
    rawTime = date.rawTime;
    gmtTime = date.gmtTime;
    strcpy(formatedDate, date.formatedDate);
}

DateTime::DateTime(const string &date) {
    // Creates DateTime object based on given string
    initializeFormats();
    string matchedFormat = "";

    if(regex_match(date, acceptedFormats[0])) {
        matchedFormat = "%a, %d %b %Y %H:%M:%S GMT";
    }
    else if(regex_match(date, acceptedFormats[1])) {
        matchedFormat = "%a, %d-%b-%y %H:%M:%S GMT";
    }
    else if(regex_match(date, acceptedFormats[2])) {
        matchedFormat = "%a %b %d %H:%M:%S %Y";
    }
    else {
        throw std::runtime_error("Unknown date format");
    }

    strptime(date.c_str(), matchedFormat.c_str(), &gmtTime);
    strncpy(formatedDate, date.c_str(), sizeof(formatedDate));
    rawTime = timegm(&gmtTime);
}

DateTime::DateTime(const struct tm& clock) {
    // Creates DateTime based on information from given struct tm
    gmtTime = clock;
    rawTime = timegm(&gmtTime);
    strftime(formatedDate, sizeof(formatedDate), "%a, %d %b %Y %H:%M:%S GMT", &gmtTime);
}

void DateTime::initializeFormats() {
    // Regular expressions used to recognise date format given in string

    // Mon, 15 Jan 2015 20:10:30 GMT
    acceptedFormats[0] = regex("^(\\u\\l{2}),\\s(\\d{1,2})\\s(\\u\\l{2})\\s(\\d{4})\\s(\\d{1,2}):(\\d{1,2}):(\\d{1,2})\\sGMT$");

    // Monday, 15-Jan-15 20:10:30 GMT
    acceptedFormats[1] = regex("^(\\u\\l+),\\s(\\d{1,2})-(\\u\\l{2})-(\\d{2})\\s(\\d{1,2}):(\\d{1,2}):(\\d{1,2})\\sGMT$");

    // Mon Jan 15 20:10:30 2015
    acceptedFormats[2] = regex("^(\\u\\l{2})\\s(\\u\\l{2})\\s(\\d{1,2})\\s(\\d{1,2}):(\\d{1,2}):(\\d{1,2})\\s(\\d{4})$");
}

const char* DateTime::getDate() const {
    return formatedDate;
}

// Simple comparation operators, based on rawTime in miliseconds
bool DateTime::operator==(const DateTime& date) const {
    return this->rawTime == date.rawTime; 
}

bool DateTime::operator<=(const DateTime& date) const {
    return this->rawTime <= date.rawTime;
}

bool DateTime::operator>(const DateTime& date) const {
    return this->rawTime > date.rawTime;
}
