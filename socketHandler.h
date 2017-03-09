//
// Created by Emil Bonne Kristiansen on 09/03/2017.
//

#ifndef NETVARKSPRAKTIK_1_SOCKETHANDLER_H
#define NETVARKSPRAKTIK_1_SOCKETHANDLER_H

using namespace std;

class socketHandler {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int messageSize = 256;
    char buffer[256];
public:
    socketHandler(string, string);
    string sendMessage(string);
    void error(string);
};


#endif //NETVARKSPRAKTIK_1_SOCKETHANDLER_H
