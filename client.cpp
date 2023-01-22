//
// Created by danas on 30/12/2022.
//

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <stdexcept>
#include <cfloat>
#include <valarray>
#include "CLI.h"
#include "Server.h"

using namespace std;

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

void checkingUserInput(string user_input, int sock, int& check) {
    vector<double> vec;
    double doubleNum;
    string str;
    int intNum;

    if(user_input == "-1"){
        close(sock);
        exit(1);
    }
    stringstream ss(user_input);
    while (ss >> doubleNum) {
        vec.push_back(doubleNum);
    }
    //check that there are numbers for the vector.
    if (vec.empty()) {
        cout << "invalid input" << endl;
        check = 1;
        return;
    }
    //check that there is a string.
    ss.clear();
    if(ss >> str){
        ss << str;
    }
    if(str.empty()){
        cout << "invalid input" << endl;
        check = 1;
        return;
    }
    //check that there is an int.
    ss.clear();
    if(ss >> intNum){
        ss << intNum;
    }
    if(!ss.eof()) {
        check = 1;
        return;
    }
}

    int main(int argc, char *argv[]) {
        string user_input;
        vector<thread*> threadVec;
        const char *ip_address = argv[1];
        const int client_port = atoi(argv[2]);
        checkingClientArgv(client_port, ip_address);
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            cout<< "error creating socket" <<endl;
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
            char buffer[4096] = {0};
            int expected_data_len = sizeof(buffer);
            //receiving the menu from the CLI
            int menu_size = 7;
            int read_bytes;
            for (int i = 0; i < menu_size; i++) {
                read_bytes = recv(sock, buffer, expected_data_len, 0);
                //if (read_bytes == 0) {
                //    cout << "no result from the server" << endl;
                //   break;
                //}
                //i added = here
                if (read_bytes <= 0) {
                    cout << "error getting a message from CLI" << endl;
                    break;
                }
                // convert buffer to a string
                string menu(buffer);
                cout << menu << endl;
            }

            //getting an option from the user and sending it to the cli
            int option;
            int data_len;
            const char *data_addr;
            cin >> option;
            int sent_bytes = send(sock, &option, sizeof(option), 0);
            if (sent_bytes < 0) {
                cout << "error sending a message" << endl;
                break;
            }

            if(option == 1) {
                read_bytes = recv(sock, buffer, expected_data_len, 0);
                if (read_bytes <= 0) {
                    cout << "error getting a message from CLI" << endl;
                    break;
                }
                // convert buffer to a string
                string trainFile;
                string file1(buffer);
                cout << file1 << endl;
                //getting the file from the user
                getline(cin, trainFile);
                data_len = trainFile.length();
                data_addr = trainFile.c_str();
                sent_bytes = send(sock, data_addr, data_len, 0);
                if (sent_bytes < 0) {
                    cout << "error sending a message" << endl;
                    break;
                }
                read_bytes = recv(sock, buffer, expected_data_len, 0);
                if (read_bytes <= 0) {
                    cout << "error getting a message from CLI" << endl;
                    break;
                }
                string message(buffer);
                if(message == "invalid input"){
                    cout << message << endl;
                    break;
                }
                else{
                    //open the file, send the lines to cli and upload it.
                    read_bytes = recv(sock, buffer, expected_data_len, 0);
                    if (read_bytes <= 0) {
                        cout << "error getting a message from CLI" << endl;
                        break;
                    }
                    string finishUploading(buffer);
                    cout << finishUploading << endl;

                    //again for the test file
                    read_bytes = recv(sock, buffer, expected_data_len, 0);
                    if (read_bytes <= 0) {
                        cout << "error getting a message from CLI" << endl;
                        break;
                    }
                    // convert buffer to a string
                    string testFile;
                    string file2(buffer);
                    cout << file2 << endl;
                    //getting the file from the user
                    getline(cin, testFile);
                    data_len = testFile.length();
                    data_addr = testFile.c_str();
                    sent_bytes = send(sock, data_addr, data_len, 0);
                    if (sent_bytes < 0) {
                        cout << "error sending a message" << endl;
                        break;
                    }
                    read_bytes = recv(sock, buffer, expected_data_len, 0);
                    if (read_bytes <= 0) {
                        cout << "error getting a message from CLI" << endl;
                        break;
                    }
                    string message2(buffer);
                    if(message2 == "invalid input"){
                        cout << message2 << endl;
                        break;
                    }
                    else{
                        //open the file, send the lines to cli and upload it.
                        read_bytes = recv(sock, buffer, expected_data_len, 0);
                        if (read_bytes <= 0) {
                            cout << "error getting a message from CLI" << endl;
                            break;
                        }
                        string finishUploading2(buffer);
                        cout << finishUploading2 << endl;
                    }
                }
            }

            if(option == 2){
                read_bytes = recv(sock, buffer, expected_data_len, 0);
                if (read_bytes <= 0) {
                    cout << "error getting a message from CLI" << endl;
                    break;
                }
                // convert buffer to a string
                string new_settings;
                string algo_settings(buffer);
                cout << algo_settings << endl;
                //get a new k and dis
                getline(cin, new_settings);
                if(new_settings.length()==0) {
                    break;
                }
                //sending it to the cli
                data_len = new_settings.length();
                data_addr = new_settings.c_str();
                sent_bytes = send(sock, data_addr, data_len, 0);
                if (sent_bytes < 0) {
                    cout << "error sending a message" << endl;
                    break;
                }

                read_bytes = recv(sock, buffer, expected_data_len, 0);
                if (read_bytes <= 0) {
                    cout << "error getting a message from CLI" << endl;
                    break;
                }
                // convert buffer to a string
                string ifValid(buffer);
                if(ifValid == "valid") {
                    break;
                }
                else {
                    cout << ifValid << endl;
                }
            }

            if(option == 3){
                read_bytes = recv(sock, buffer, expected_data_len, 0);
                if (read_bytes <= 0) {
                    cout << "error getting a message from CLI" << endl;
                    break;
                }
                // convert buffer to a string
                string classify(buffer);
                cout<< classify<< endl;
            }

            if(option == 4){
                read_bytes = recv(sock, buffer, expected_data_len, 0);
                if (read_bytes <= 0) {
                    cout << "error getting a message from CLI" << endl;
                    break;
                }
                // convert buffer to a string
                string print_classify(buffer);
                if(print_classify == "please upload data." || print_classify == "please classify data.") {
                    cout<< print_classify<< endl;
                    break;
                }
                while(print_classify != "Done."){
                    cout<< print_classify << endl;
                    read_bytes = recv(sock, buffer, expected_data_len, 0);
                    if (read_bytes <= 0) {
                        cout << "error getting a message from CLI" << endl;
                        break;
                    }
                    // convert buffer to a string
                    string print_classify(buffer);
                }
                cout<< print_classify << endl;
                //waiting for enter from the user
                getline(cin, user_input);
                if (user_input.empty()) {
                    break;
                }
            }

            if(option == 5) {
                thread* t = new thread() {
                    threadVec.push_back(t);
                    string local_path;
                    //get a local path from user
                    getline(cin, local_path);
                    ofstream MyFile(local_path);
                    read_bytes = recv(sock, buffer, expected_data_len, 0);
                    if (read_bytes <= 0) {
                        cout << "error getting a message from CLI" << endl;
                        break;
                    }
                    // convert buffer to a string
                    string write_classify(buffer);
                    if(write_classify == "please upload data." || write_classify == "please classify data.") {
                        cout<< write_classify<< endl;
                        break;
                    }
                    int i = 1;
                    while(write_classify != "Done."){
                        //write to file
                        MyFile << i + " " + write_classify << endl;
                        read_bytes = recv(sock, buffer, expected_data_len, 0);
                        if (read_bytes <= 0) {
                            cout << "error getting a message from CLI" << endl;
                            break;
                        }
                        // convert buffer to a string
                        string print_classify(buffer);
                        i++;
                    }
                    MyFile.close();
                }
            }

            if(option == 8) {
                //check if connection is must
                read_bytes = recv(sock, buffer, expected_data_len, 0);
                if (read_bytes <= 0) {
                    cout << "error getting a message from CLI" << endl;
                    break;
                }
                string exit(buffer);
                if(exit == "close") {
                    close(sock);
                }
            }
        }
    }


