//
// Created by danas on 1/19/2023.
//

#ifndef MULTCLIENTREP_COMMANDS_H
#define MULTCLIENTREP_COMMANDS_H


#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


using namespace std;

class DeafultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DeafultIO(){}

};

class SocketIO: public DeafultIO{
private:
    int clientID;
public:
    SocketIO(int ID){
        this->clientID = ID;
    }
    string read() override{
        char buffer[4096] = {0};
        int expected_data_len = sizeof(buffer);
        int read_bytes = recv(clientID, buffer, expected_data_len, 0);
        if (read_bytes == 0) {
            cout << "no message from client" << endl;
            return "";
        }
        if (read_bytes < 0) {
            cout << "error getting a message from client" << endl;
            return "";
        }
        string data = buffer;
        return data;
    }

    void write(string text) override{
        int length = text.length();
        char message_to_send[length + 1];
        strcpy(message_to_send, text.c_str());
        int send_bytes = send(clientID, message_to_send, length, 0);
        if (send_bytes < 0) {
            cout << "error sending a message" << endl;
        }
    }

    void write ()

};

#endif //MULTCLIENTREP_COMMANDS_H



