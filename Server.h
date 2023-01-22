//
// Created by omern on 21/01/2023.
//

#ifndef MULTCLIENTREP_SERVER_H
#define MULTCLIENTREP_SERVER_H

#include <pthread.h>
#include <thread>
#include "commands.h"
#include "CLI.h"



class SocketIO: public DeafultIO{
private:
    int clientID;
public:
    explicit SocketIO(int ID){
        this->clientID = ID;
    }
    //read string from client
    string read() override {
        string message;
        char c;
        int expected_data_len = sizeof(c);
        while(c != '@') {
            int read_bytes = recv(clientID, &c, expected_data_len, 0);
            if(c == '@') {
                break;
            }
            if (read_bytes == 0) {
                cout << "no message from client" << endl;
                exit(1);
            }
            if (read_bytes < 0) {
                cout << "error getting a message from client" << endl;
                exit(1);
            }
            message += c;
        }
        return message;
//        string message;
//        char buffer[4096] = {0};
//        int expected_data_len = sizeof(buffer);
//        int read_bytes = recv(clientID, &buffer, expected_data_len, 0);
//        if (read_bytes == 0) {
//            cout << "no message from client" << endl;
//            return message;
//        }
//        if (read_bytes < 0) {
//            cout << "error getting a message from client" << endl;
//            exit(0);
//        }
//        message = buffer;
//        return message;
    }

    //write string to client
    void write(string text) override {
        int length = text.length();
        text[length] = '@';
        char message_to_send[length + 1];
        strcpy(message_to_send, text.c_str());
        int send_bytes = send(clientID, message_to_send, length + 1, 0);
        if (send_bytes < 0) {
            cout << "error sending a message" << endl;
            return;
        }
//        int length = text.length();
//        char message_to_send[length + 1];
//        strcpy(message_to_send, text.c_str());
//        int send_bytes = send(clientID, message_to_send, length, 0);
//        if (send_bytes < 0) {
//            cout << "error sending a message" << endl;
//            return;
//        }
    }
};

class ClientHandler{
public:
    virtual void handle(int clientID){
        SocketIO socket(clientID);
        CLI cli(&socket);
        cli.start();
    }
};

class Server {
    int sockNum;
    sockaddr_in server;
    sockaddr_in client_sin;

public:
    explicit Server(int port);
    void start(ClientHandler& ch);
};


#endif //MULTCLIENTREP_SERVER_H
