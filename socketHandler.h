//
// Created by Emil Bonne Kristiansen on 09/03/2017.
//

#ifndef NETVARKSPRAKTIK_1_SOCKETHANDLER_H
#define NETVARKSPRAKTIK_1_SOCKETHANDLER_H

#include "messageReply.h"

using namespace std;

// This class is responsible for handling a socket
class socketHandler {
private:
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int messageSize = 1024;
public:
    char buffer[1024];
    socketHandler();
    socketHandler(string, string);
    void setConnection(string, string);
    void closeConnection();
    void reconnect(string, string);
    void sendFile(string);
    void send(string);
    string receive();
    string receiveFile(string);
    messageReply sendMessage(string);
    void error(string);
};


#endif //NETVARKSPRAKTIK_1_SOCKETHANDLER_H
