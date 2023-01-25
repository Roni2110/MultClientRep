//
// Created by danas on 30/12/2022.
//

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <valarray>
#include "Server.h"


using namespace std;

void writeByChar(string text, int serverId) {
    int length = text.length();
    text[length] = '@';
    char message_to_send[length+1];
    strcpy(message_to_send, text.c_str());
    int send_bytes = send(serverId, message_to_send, sizeof(message_to_send), 0);
    if (send_bytes < 0) {
        cout << "error sending a message" << endl;
        return;
    }
}

string readByChar(int serverId) {
    string message;
    char c;
    int expected_data_len = sizeof(c);
    while(c != '@') {
        int read_bytes = recv(serverId, &c, expected_data_len, 0);
        if(c == '@') {
            break;
        }
        if (read_bytes == 0) {
            cout << "no message from server" << endl;
            exit(1);
        }
        if (read_bytes < 0) {
            cout << "error getting a message from server" << endl;
            exit(1);
        }
        message += c;
    }
    return message;
}

/**
 * checking arguments for the client.
 * @param port - the port number - should be between 1 to 65535.
 * @param ip_address - the IP address - should be in IP4 format.
 */
void checkingClientArgv(int port, string ip_address){
    uint32_t sa;
    if(port < 1024 || port > 65535) {
        cout << "invalid port number!" << endl;
        exit(1);
    }
    int checkIP = inet_pton(AF_INET, ip_address.c_str() ,&sa);
    if (checkIP != 1){
        cout << "invalid IP address!" << endl;
        exit(1);
    }
}

void writeFile(string path, vector<string>v1) {
    ofstream outFile;
    outFile.open(path);
    int size = v1.size();
    for(int i = 0; i < size - 1; i++) {
        outFile << to_string(i+1) + " " + v1.at(i) << endl;
    }
    outFile.close();
}

static int checkInput(string input) {
    string str2 = ".csv";
    if(!(strstr(input.c_str(), str2.c_str()))) {
        cout << "invalid input" << endl;
        return -1;
    }
    //file pointer
    fstream fin;
    //open an existing file
    fin.open(input);
    if(!fin.is_open()) {
        cout << "Cant open file" << endl;
        return -1;
    }
    return 0;
}

void sendingFiles(string path, int socket) {
    vector<vector<double>> resVec;
    vector<string> stringVec;
    string tempByLine, tempByComma;
    //file pointer
    fstream fin;
    //open an existing file
    fin.open(path);
    while(getline(fin, tempByLine)) {
        writeByChar(tempByLine,socket);
    }
    writeByChar("finish",socket);
    fin.close();
}

int main(int argc, char *argv[]) {
    bool toFinish = false;
    string user_input;
    vector<thread> threadVec;
    const char *ip_address = argv[1];
    const int client_port = atoi(argv[2]);
    checkingClientArgv(client_port, ip_address);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cout << "error creating socket" << endl;
        exit(1);
    }
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(client_port);
    if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        cout << "error connecting to server" << endl;
        exit(1);
    }
    while (true) {
        string temp;
        //receiving the menu from the CLI
        int menu_size = 7;
        for (int i = 0; i < menu_size; i++) {
            temp = readByChar(sock);
            cout << temp;
        }

        //getting an option from the user and sending it to the cli
        string option;
        cin >> option;
        if(option != "1" && option != "2" && option != "3" && option != "4" && option != "5" &&
        option != "8"){
            cout << "invalid input" << endl;
            writeByChar("-1" , sock);
            continue;
        }
        writeByChar(option,sock);

        if (option == "1") {
            temp = readByChar(sock);
            cout << temp << endl; //please upload train
            string trainFile;
            int valid;
            //getting the file from the user
            cin.ignore();
            getline(cin, trainFile);
            valid = checkInput(trainFile);
            if (valid == -1) {
                writeByChar("stop", sock);
                continue;
            }
            //open train file
            sendingFiles(trainFile,sock);
            temp = readByChar(sock); //upload train complete
            cout << temp << endl;

            temp = readByChar(sock);
            cout << temp << endl; //please upload test
            string testFile;
            cin>>std::ws;
            getline(cin, testFile); //getting path test
            valid = checkInput(testFile);
            if (valid == -1) {
                writeByChar("stop", sock);
                continue;
            }
            //open test file
            sendingFiles(testFile,sock);
            temp = readByChar(sock); //upload test complete
            cout << temp << endl;
        }
        if (option == "2") {
            temp = readByChar(sock);
            cout << temp << endl;
            string new_settings;
            //get a new k and dis
            cin.ignore();
            getline(cin, new_settings);
            if (new_settings.length() == 0) { //case the input is \n
                writeByChar(new_settings,sock);
                continue;
            }
            //sending it to the cli
            writeByChar(new_settings,sock);
            temp = readByChar(sock);
            if (temp == "valid") {
                continue;
            } else {
                cout << temp << endl;
            }
        }

        if (option == "3") {
            temp = readByChar(sock);
            cout << temp << endl;
        }

        if (option == "4") {
            temp = readByChar(sock);
            if (temp == "please upload data." || temp == "please classify data.") {
                cout << temp << endl;
                continue;
            }
            int i = 1;
            while (temp != "Done.") {
                cout << to_string(i) + " " + temp << endl;
                temp = readByChar(sock);
                i++;
            }
            cout << temp << endl;
            //waiting for enter from the user
            cin.ignore();
            getline(cin, user_input);
            while(!user_input.empty()) {
                cin.ignore();
                getline(cin, user_input);
            }
        }

        if (option == "5") {
            string local_path;
            vector<string> v1;
            //get a local path from user
            cin.ignore();
            getline(cin, local_path);
            temp = readByChar(sock);
            if (temp == "please upload data." || temp == "please classify data.") {
                cout << temp << endl;
                continue;
            }
            if (temp == "Done.") {
                continue;
            }
            v1.push_back(temp);
            while (temp != "Done.") {
                temp = readByChar(sock);
                v1.push_back(temp);
            }
            //a new thread writing to the file
            threadVec.emplace_back(writeFile, local_path, v1);
        }

        if (option == "8") {
            temp = readByChar(sock);
            if (temp == "close") {
                toFinish = true;
                close(sock);
            }
        }
        if (toFinish) {
            break;
        }
    }
    for(int i = 0; i < threadVec.size(); i++) {
        threadVec.at(i).join();
    }
}


