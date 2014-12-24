#include "DateTime.h"
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
    acceptedFormats[0] = regex("^\\u\\l{2},\\s\\d{1,2}\\s\\u\\l{2}\\s\\d{4}\\s\\d{1,2}:\\d{1,2}:\\d{1,2}\\sGMT$");
    acceptedFormats[1] = regex("^\\u\\l+,\\s\\d{1,2}-\\u\\l{2}-\\d{2}\\s\\d{1,2}:\\d{1,2}:\\d{1,2}\\sGMT$");
    acceptedFormats[2] = regex("^\\u\\l{2}\\s\\u\\l{2}\\s\\d{1,2}\\s\\d{1,2}:\\d{1,2}:\\d{1,2}\\s\\d{4}$");
    
    if(regex_match("Fri Dec 31 7:12:1 2014", acceptedFormats[2])) {
        cout << "T" << endl;
    }
    else {
        cout << "N" << endl;
    }
}

string DateTime::getDate() {
    return formatedDate;
}
