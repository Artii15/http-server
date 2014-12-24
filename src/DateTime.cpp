#include "DateTime.h"
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

DateTime::DateTime() {
    rawTime = time(NULL);  
    gmtTime = gmtime(&rawTime);
    strftime(formatedDate, sizeof(formatedDate), "%a, %d %b %Y %H:%M:%S GMT", gmtTime);
}

DateTime::DateTime(const string &date) {
    regex pattern1("asdas");
}

string DateTime::getDate() {
    return formatedDate;
}
