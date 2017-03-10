//
// Created by Emil Bonne Kristiansen on 09/03/2017.
//

#ifndef NETVARKSPRAKTIK_1_SOCKETHANDLER_H
#define NETVARKSPRAKTIK_1_SOCKETHANDLER_H

#include "messageReply.h"

using namespace std;

class socketHandler {
private:
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int messageSize = 1024;
    char buffer[1024];
public:
    socketHandler(string, string);
    void setConnection(string, string);
    void send(string);
    string receive();
    messageReply sendMessage(string);
    void error(string);
    void closeConnection();
};


#endif //NETVARKSPRAKTIK_1_SOCKETHANDLER_H
