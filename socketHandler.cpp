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

// Empty constructor
socketHandler::socketHandler() {

}

// Constructor for connection immediatly
socketHandler::socketHandler(string hostname, string port) {
    setConnection(hostname, port);
};

// Send the contents of a file to server
void socketHandler::sendFile(string filename) {
    FILE *pFile;
    pFile = fopen(filename.c_str(), "r");
    bzero(buffer, messageSize);
    fgets(buffer, messageSize, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        error("Could not write to socket");
}

// Send a message to server
void socketHandler::send(string message) {
    message.append("\n");
    bzero(buffer, messageSize);
    strncpy(buffer, message.c_str(), messageSize);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        error("Could not write to socket");
}

// Receive a message from server
string socketHandler::receive() {
    bzero(buffer, messageSize);
    n = read(sockfd, buffer, messageSize);
    if (n < 0)
        error("Could not read from socket");
    string returnvalue(buffer);
    bzero(buffer, messageSize);
    return returnvalue;
};

// Receive a file from server
string socketHandler::receiveFile(string filename) {
    string returnvalue;
    int kbs = 0;
    bool messageGoesOn = false;
    FILE *pFile;
    pFile = fopen(filename.c_str(), "w");
    do {
        kbs++;
        bzero(buffer, messageSize);
        n = read(sockfd, buffer, messageSize);
        if (n < 0)
            error("Could not read from socket");
        if (kbs == 1) {
            string placeholder(buffer);
            returnvalue = placeholder;
        }
        int i;
        for (i = 0; i<messageSize; i++) {
            messageGoesOn = false;
            if (buffer[i] != '\0') {
                messageGoesOn = true;
                break;
            }
        }
        fprintf(pFile, buffer);
    } while(messageGoesOn);
    fclose(pFile);
    return returnvalue;
};

// Send a message and receive answer from server
messageReply socketHandler::sendMessage(string message) {
    send(message);
    return messageReply(receive());
};

// Write out error message and close program
void socketHandler::error(string errorMessage) {
    perror(errorMessage.c_str());
    exit(0);
}

// Setup connection to server
void socketHandler::setConnection(string hostname, string port) {
    server = gethostbyname(hostname.c_str());
    portno = atoi(port.c_str());

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Could not open socket, maybe the port is blocked?");

    // Validate server
    if (server == NULL) {
        error("Could not find server");
    }

    // Doing important pointer-stuff
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
          (char *) &serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Kunne ikke forbinde");
}

// Close connection ro server
void socketHandler::closeConnection() {
    close(sockfd);
}

// Reconnect to server
void socketHandler::reconnect(string hostname, string port) {
    closeConnection();
    setConnection(hostname, port);
}




