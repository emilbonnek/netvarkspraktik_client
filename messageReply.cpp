//
// Created by Emil Bonne Kristiansen on 10/03/2017.
//

#include <regex>
#include "messageReply.h"

using namespace std;

messageReply::messageReply() {
    code = 0;
    message = "";
}

messageReply::messageReply(const string s) {
    regex rgx1("(\\d+)(.+)");
    smatch match;

    if (regex_search(s.begin(), s.end(), match, rgx1)) {
        code = stoi(match[1]);
        message = match[2];
    }
}

int messageReply::getCode() {
    return code;
}

string messageReply::getMessage() {
    return message;
}

string messageReply::format() {
    return to_string(getCode())+message;
}
