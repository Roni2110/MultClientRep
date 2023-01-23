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
#include <fstream>
#include "Server.h"


using namespace std;

void writeByChar(string text, int serverId) {
    int length = text.length();
    text[length] = '@';
    char message_to_send[length+1];
    cout<< sizeof(message_to_send) <<endl;
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

void writeFile(string path, string* str1) {
    ofstream outFile;
    outFile.open(path);
    int j = 1;
    for(int i = 0; i < str1->length(); i++) {
        outFile << to_string(j) + " " + str1->at(i) << endl;
    }
    outFile.close();
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
        writeByChar(option,sock);

        if (option == "1") {
            cout<<"hiii"<<endl;
            temp = readByChar(sock);
            cout<<temp<<endl;
            cout << temp << endl; //please upload train
            string trainFile;
            //getting the file from the user
            getline(cin, trainFile);
            writeByChar(trainFile,sock); //send train path
            temp = readByChar(sock); //invalid or complete
            if (temp == "invalid input") {
                cout << temp << endl;
                break;
            } else {
                cout << temp << endl; //complete
                temp = readByChar(sock); //upload test file
                cout << temp << endl;
                string testFile;
                //getting the file from the user
                getline(cin, testFile);
                writeByChar(testFile,sock);
                temp = readByChar(sock); //invalid or complete
                if (temp == "invalid input") {
                    cout << temp << endl;
                    break;
                } else {
                    cout << temp << endl;
                }
            }
        }

        if (option == "2") {
            temp = readByChar(sock);
            cout << temp << endl;
            string new_settings;
            //get a new k and dis
            getline(cin, new_settings);
            if (new_settings.length() == 0) {
                break;
            }
            //sending it to the cli
            writeByChar(new_settings,sock);
            temp = readByChar(sock);
            if (temp == "valid") {
                break;
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
                break;
            }
            while (temp != "Done.") {
                cout << temp << endl;
                temp = readByChar(sock);
            }
            cout << temp << endl;
            //waiting for enter from the user
            getline(cin, user_input);
            if (user_input.empty()) {
                break;
            }
        }

        if (option == "5") {
            string local_path;
            string str[4096] = {nullptr};
            int i = 1;
            //get a local path from user
            getline(cin, local_path);
            temp = readByChar(sock);
            if (temp == "please upload data." || temp == "please classify data.") {
                cout << temp << endl;
                break;
            }
            if (temp == "Done.") {
                break;
            }
            str[0] = temp;
            while (temp != "Done.") {
                temp = readByChar(sock);
                str[i] = temp;
                i++;
            }
            //a new thread writing to the file
            threadVec.emplace_back(writeFile, local_path, str);
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


