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

// Calculates the port for dataconnectivity given a PASV response from the server
int calculatePort(const string s) {
    int port = 256;
    regex rgx("(\\d+),(\\d+)\\)"); // Regex for finding port number given a response from the server
    smatch match;

    if (regex_search(s.begin(), s.end(), match, rgx)) {
        // As match saves it as basic_string we will need to save it as just a string before we can make it into a an int
        string str = match[1];
        port *= stoi(str);
        str = match[2];
        port += stoi(str);
    }

    return port;
}

int main(int argc, char *argv[]) {
    // Fejlmeddelse hvis programmet bruges forkert (forkert antal argumenter)
    if (argc < 5) {
        fprintf(stderr, "usage: %s hostname port username password\n", argv[0]);
        exit(0);
    }
    // Gem programmets argumenter i variabler
    string hostname(argv[1]);
    string port(argv[2]);
    string username(argv[3]);
    string password(argv[4]);

    messageReply controlResponse;
    string dataResponse;
    socketHandler controlCon(hostname, port);
    int newPort;
    socketHandler dataCon;

    // Send default beskeder
    controlCon.sendMessage("");                 // Tom besked til at initiere kontakt
    controlCon.sendMessage("USER " + username); // Angiv brugernavn
    controlCon.sendMessage("PASS " + password); // Angiv password
    controlCon.sendMessage("TYPE A");           // Angiv at dataforbindelsen skal benytte ASCII

    // Opsæt dataforbindelse
    controlResponse = controlCon.sendMessage("PASV"); // Sæt serveren i PASSIVE mode
    newPort = calculatePort(controlResponse.getMessage());
    dataCon.reconnect(hostname, to_string(newPort));

    // EKSEMPLER
    // 1. Hent en fil fra roden
    cout << "------EKSEMPEL 1------" << endl;
    controlCon.sendMessage("RETR file.txt");
    dataResponse = dataCon.receiveToFile("../downloads/file.txt");
    cout << dataResponse << endl;
    controlCon.sendMessage(""); // En tom besked sendes
    // Genskab dataforbindelse
    controlResponse = controlCon.sendMessage("PASV");
    newPort = calculatePort(controlResponse.getMessage());
    dataCon.reconnect(hostname, to_string(newPort));

    // 2. Hent en fil fra en undermappe
    cout << "------EKSEMPEL 2------" << endl;
    controlCon.sendMessage("RETR pub/62501/examples/getsatpos.h");
    dataResponse = dataCon.receiveToFile("../downloads/getsatpos.h");
    cout << dataResponse << endl;
    controlCon.sendMessage(""); // En tom besked sendes
    // Genskab dataforbindelse
    controlResponse = controlCon.sendMessage("PASV");
    newPort = calculatePort(controlResponse.getMessage());
    dataCon.reconnect(hostname, to_string(newPort));

    // 3. Forsøg at sende en fil
    cout << "------EKSEMPEL 3------" << endl;
    controlResponse = controlCon.sendMessage("STOR myfile.txt");
    if (controlResponse.getCode() == 550) {
        cout << controlResponse.format() << endl;
    } else {
        dataCon.sendFile("myfile.txt");
    }

    controlCon.sendMessage("QUIT");  // Tom besked til at initiere kontakt
    controlCon.closeConnection();
    dataCon.closeConnection();

    return 0;
}