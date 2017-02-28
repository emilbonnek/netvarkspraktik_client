#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Print error and exit program
void error(const char *msg) {
    perror(msg);
    exit(0);
}


int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int messageSize = 1000*1024;
    char buffer[messageSize];

    // Program was called with too few arguments
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port username password\n", argv[0]);
        exit(0);
    }
    // Extract server and portno from arguments
    server = gethostbyname(argv[1]);
    portno = atoi(argv[2]);

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

    // Initiate contact by sending empty TCP-message (solves the one-off problem)
    bzero(buffer, messageSize);
    write(sockfd, buffer, strlen(buffer));
    read(sockfd, buffer, messageSize);
    printf("%s\n", buffer);

    // Login
    // log ind / giv fejlmeddelese(til STDERR) og luk program todo

    // Aktivér passive mode på server
    // serveren returnerer hvordan den gerne vil have at vi forbinder til den med data-con, det skal gemmes todo

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