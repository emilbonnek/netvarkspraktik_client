//
// Created by Emil Bonne Kristiansen on 09/03/2017.
//

#include <algorithm>
#include <string>
#include <cstdlib>
#include <netdb.h>
#include <zconf.h>
#include "socketHandler.h"

using namespace std;

socketHandler::socketHandler(string hostname, string port) {
    server = gethostbyname(hostname.c_str());
    portno = atoi(port.c_str());

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    // Validate server
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    // Doing important pointer-stuff
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        printf("noget gik galt");
};

string socketHandler::sendMessage(string message) {
    message.append("\n");
    bzero(buffer, messageSize);
    strncpy(buffer, message.c_str(),messageSize);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
      error("ERROR writing to socket");
    bzero(buffer, messageSize);
    n = read(sockfd, buffer, messageSize);
    string returnvalue(buffer);
    bzero(buffer, messageSize);
    if (n < 0)
      error("ERROR reading from socket");
    return returnvalue;
};

void socketHandler::error(string errorMessage) {
    perror(errorMessage.c_str());
    exit(0);
}