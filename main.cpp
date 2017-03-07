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

using namespace std;

// Print error and exit program
void error(const char *msg) {
    perror(msg);
    exit(0);
}
void sendMessage(string message){


}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int messageSize = 256;
    char buffer[messageSize];

    // Program was called with too few arguments
    if (argc < 5) {
        fprintf(stderr,"usage %s hostname port username password\n", argv[0]);
        exit(0);
    }
    // Extract server and portno from arguments
    server = gethostbyname(argv[1]);
    portno = atoi(argv[2]);
    string username(argv[3]);
    string password(argv[4]);

    // Opening socket
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

    // Connect to server
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    // Write out commands
    cout << "USER "+username << endl;
    cout << "PASS "+password << endl;
    cout << "PASV" << endl;

    // Initiate contact by sending empty TCP-message (solves the one-off problem)
    bzero(buffer, messageSize);
    write(sockfd, buffer, strlen(buffer));
    bzero(buffer, messageSize);
    read(sockfd, buffer, messageSize);
    printf("%s\n", buffer);

    // Login

    cout << "Please enter the message: " << endl;


    // Kan du lave et  tomt FILE * objekt? et

    bzero(buffer, messageSize);
    FILE* loginCommand = fopen("loginCommandFile.txt", "r");
    //fputs(username.c_str(), loginCommand);

    //fputs(username.c_str(), stdout);
    fgets(buffer, messageSize, loginCommand);
    fputs(buffer, stdout);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
    fputs(buffer, stdout);
    bzero(buffer, messageSize);
    n = read(sockfd, buffer, messageSize);
    if (n < 0)
        error("ERROR reading from socket");
    fputs(buffer, stdout);


//
//    bzero(buffer, messageSize);
//
//    FILE* loginCommand;
//    loginCommand = fopen("loginCommandFile.txt", "a");
//    fgets(buffer, messageSize, loginCommand);
//
//    write(sockfd, buffer, strlen(buffer));
//    bzero(buffer, messageSize);
    //read(sockfd, buffer, messageSize);


    // log ind / giv fejlmeddelese(til STDERR) og luk program todo

    // Aktivér passive mode på server
    // serveren returnerer hvordan den gerne vil have at vi forbinder til den med data-con, det skal gemmes todo

    // Lav nyt socket til datakommunikation todo

    // Tekst interface
    // Kør et tekst interface ala. det der er blevet lavet i java til CDIO todo

    while (1) {
        printf("Please enter the message: ");

        bzero(buffer, messageSize);
        fgets(buffer, messageSize, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
            error("ERROR writing to socket");
        bzero(buffer, messageSize);
        n = read(sockfd, buffer, messageSize);
        if (n < 0)
            error("ERROR reading from socket");
        printf("%s\n", buffer);
    }
    close(sockfd);
    return 0;
}