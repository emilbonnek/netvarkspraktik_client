//
// Created by Emil Bonne Kristiansen on 10/03/2017.
//

#include <regex>
#include "messageReply.h"

using namespace std;

// Empty constructer
messageReply::messageReply() {
    code = 0;
    message = "";
}

// initiatiting constructer, Identifies response code and message from string
messageReply::messageReply(const string s) {
    regex rgx1("(\\d+)(.+)");
    smatch match;

    if (regex_search(s.begin(), s.end(), match, rgx1)) {
        code = stoi(match[1]);
        message = match[2];
    }
}

// getter for code
int messageReply::getCode() {
    return code;
}

// getter for message
string messageReply::getMessage() {
    return message;
}

// formats code with message as it was initially
string messageReply::format() {
    return to_string(getCode())+message;
}
