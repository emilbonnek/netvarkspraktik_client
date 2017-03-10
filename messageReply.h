//
// Created by Emil Bonne Kristiansen on 10/03/2017.
//

#ifndef NETVARKSPRAKTIK_1_MESSAGEREPLY_H
#define NETVARKSPRAKTIK_1_MESSAGEREPLY_H

#include <algorithm>
#include <string>

using namespace std;

// This class represents a reply to a message on the control connection
class messageReply {
private:
    int code;
    string message;
public:
    messageReply();
    messageReply(const string);
    int getCode();
    string getMessage();
    string format();
};


#endif //NETVARKSPRAKTIK_1_MESSAGEREPLY_H
