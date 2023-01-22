//
// Created by omern on 21/01/2023.
//

#ifndef MULTCLIENTREP_SERVER_H
#define MULTCLIENTREP_SERVER_H

#include <pthread.h>
#include <thread>
#include "commands.h"
#include "CLI.h"


class clientHandler{
public:
    virtual void handle(int clientID){
        SocketIO socket(clientID);
        CLI cli(&socket);
        cli.start();
    }
};

class SocketIO: public DeafultIO{
private:
    int clientID;
public:
    SocketIO(int ID){
        this->clientID = ID;
    }
    //read string from client
    virtual string read() {
        string message = "";
        char buffer[4096] = {0};
        int expected_data_len = sizeof(buffer);
        int read_bytes = recv(clientID, &buffer, expected_data_len, 0);
        if (read_bytes == 0) {
            cout << "no message from client" << endl;
            return message;
        }
        if (read_bytes < 0) {
            cout << "error getting a message from client" << endl;
            exit(0);
        }
        message = buffer;
        return message;
    }

    //write string to client
    virtual void write(string text) {
        int length = text.length();
        char message_to_send[length + 1];
        strcpy(message_to_send, text.c_str());
        int send_bytes = send(clientID, message_to_send, length, 0);
        if (send_bytes < 0) {
            cout << "error sending a message" << endl;
            return;
        }
    }

};

class Server {
    thread* t;
    int sockNum;
    sockaddr_in server;
    sockaddr_in client;

public:
    Server(int port) throw(const char*);
    virtual ~Server();

};


#endif //MULTCLIENTREP_SERVER_H
