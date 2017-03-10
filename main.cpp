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
#include <regex>
#include "socketHandler.h"
#include "messageReply.h"


using namespace std;

int calculatePort(const string s) {
    int port = 256;
    string str;

    regex rgx("(\\d+),(\\d+)\\)");
    smatch match;

    if (regex_search(s.begin(), s.end(), match, rgx)) {
        str = match[1];
        port *= atoi(str.c_str());
        str = match[2];
        port += atoi(str.c_str());
    }

    return port;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "usage: %s hostname port username password\n", argv[0]);
        exit(0);
    }
    // Extract server and portno from arguments
    string hostname(argv[1]);
    string port(argv[2]);
    string username(argv[3]);
    string password(argv[4]);

    messageReply controlResponse;
    string dataResponse;
    socketHandler controlCon(hostname, port);

    // Send default beskeder
    controlCon.sendMessage("");
    controlCon.sendMessage("USER " + username);
    controlCon.sendMessage("PASS " + password);
    controlResponse = controlCon.sendMessage("PASV");

    // Beregn port til datafobindelse og opret nyt socket
    int newPort = calculatePort(controlResponse.getMessage());
    socketHandler dataCon(hostname, to_string(newPort));


    // Lad brugeren interegere
    string inputString;

    cout << "Skriv en besked" << endl;
    getline(cin, inputString);
    controlResponse = controlCon.sendMessage(inputString);
    cout << "--CONTROL CONNECTION REPLY--" << endl;
    cout << controlResponse.format() << endl;
    cout << "--DATA REPLY--" << endl;
    dataResponse = dataCon.receive();
    cout << dataResponse << endl;

    // Skriv dataresponse til fil todo

    controlCon.closeConnection();
    dataCon.closeConnection();

    return 0;
}