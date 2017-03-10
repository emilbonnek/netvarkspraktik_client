//
// Created by Emil Bonne Kristiansen on 09/03/2017.
//

#include <algorithm>
#include <string>
#include <cstdlib>
#include <netdb.h>
#include <zconf.h>
#include "socketHandler.h"
#include "messageReply.h"

using namespace std;

socketHandler::socketHandler(string hostname, string port) {
    setConnection(hostname, port);
};

// Send besked
void socketHandler::send(string message){
    message.append("\n");
    bzero(buffer, messageSize);
    strncpy(buffer, message.c_str(),messageSize);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        error("Kunne ikke skrive til socket");
}

// Modtag besked
string socketHandler::receive(){
    bzero(buffer, messageSize);
    n = read(sockfd, buffer, messageSize);
    string returnvalue(buffer);
    bzero(buffer, messageSize);
    if (n < 0)
        error("Kunne ikke læse fra socket");
    return returnvalue;
};
// Send besked og modtag svar
messageReply socketHandler::sendMessage(string message) {
    send(message);
    return messageReply(receive());
};

// Udskriv fejl og luk progam
void socketHandler::error(string errorMessage) {
    perror(errorMessage.c_str());
    exit(0);
}

// Lav forbindelse
void socketHandler::setConnection(string hostname, string port){
    server = gethostbyname(hostname.c_str());
    portno = atoi(port.c_str());

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Kunne ikke åbne socket, måske er porten optaget?");

    // Validate server
    if (server == NULL) {
        error("Kunne ikke finde serveren");
    }

    // Doing important pointer-stuff
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("Kunne ikke forbinde");
}

// luk forbindelse
void socketHandler::closeConnection() {
    close(sockfd);
}

