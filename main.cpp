#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <fstream>
#include "socketHandler.h"

using namespace std;

// Print error and exit program
void error(const char *msg) {
    perror(msg);
    exit(0);
}
string sendMessage(string message){

}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr,"usage %s hostname port username password\n", argv[0]);
        exit(0);
    }
    // Extract server and portno from arguments
    string hostname(argv[1]);
    string port(argv[2]);
    string username(argv[3]);
    string password(argv[4]);

    string response;

    socketHandler rect(hostname,port);
    response = rect.sendMessage("");
    response = rect.sendMessage("USER "+username);
    response = rect.sendMessage("PASS "+password);
    response = rect.sendMessage("PASV");
    cout << response;

//
//    // log ind / giv fejlmeddelese(til STDERR) og luk program todo
//
//    // Aktivér passive mode på server
//    // serveren returnerer hvordan den gerne vil have at vi forbinder til den med data-con, det skal gemmes todo
//
//    // Lav nyt socket til datakommunikation todo
//
//    // Tekst interface
//    // Kør et tekst interface ala. det der er blevet lavet i java til CDIO todo
//
//    while (1) {
//        printf("Please enter the message: ");
//        bzero(buffer, messageSize);
//        fgets(buffer, messageSize, stdin);
//        n = write(sockfd, buffer, strlen(buffer));
//        if (n < 0)
//            error("ERROR writing to socket");
//        bzero(buffer, messageSize);
//        n = read(sockfd, buffer, messageSize);
//        if (n < 0)
//            error("ERROR reading from socket");
//        printf("%s\n", buffer);
//    }
//    close(sockfd);
//    return 0;
}