#include "DateTime.h"
#include <boost/regex.hpp>
#include <string>

using namespace std;
using namespace boost;

DateTime::DateTime() {
    initializeFormats(); 
    rawTime = time(NULL);  
    gmtTime = gmtime(&rawTime);
    strftime(formatedDate, sizeof(formatedDate), "%a, %d %b %Y %H:%M:%S GMT", gmtTime);
}

DateTime::DateTime(const string &date) {
    initializeFormats();
    regex pattern1("asdas");
}

void DateTime::initializeFormats() {
    knownFormats[0] = "^\\u\\l{2},\\s\\d{1,2}\\s\\u\\l{2}\\s\\d{4}\\s\\d{1,2}:\\d{1,2}:\\d{1,2}\\sGMT$";
    knownFormats[1] = "";
    knownFormats[2] = "";
    
    if(regex_match("Wed, 24 Dec 2014 14:42:1 GMT", regex(knownFormats[0]))) {
        cout << "T" << endl;
    }
    else {
        cout << "N" << endl;
    }
}

string DateTime::getDate() {
    return formatedDate;
}
