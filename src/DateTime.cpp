#include "DateTime.h"
#include <iostream>

using namespace std;

DateTime::DateTime() {
    rawTime = time(NULL);  
    gmtTime = gmtime(&rawTime);
    strftime(formatedDate, sizeof(formatedDate), "%a, %d %b %Y %H:%M:%S GMT", gmtTime);
}

string DateTime::getDate() {
    return formatedDate;
}
