//
// Created by danas on 1/19/2023.
//

#ifndef MULTCLIENTREP_COMMANDS_H
#define MULTCLIENTREP_COMMANDS_H


#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>


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

};

class Command{
protected:
    DeafultIO* dio;
    string description;

public:
    Command(DeafultIO* dio): dio(dio){}
    virtual void execute()=0;
    virtual ~Command(){}
    virtual void print(){
        dio->write(description);
    }
};

/**
 * first command.
 */
class UploadFiles : public Command {
public:

    UploadFiles(DeafultIO* dio) : Command(dio){
        this->description = "1. upload an unclassified csv data files\n";
    }

    /**
     * destructor.
     */
    virtual ~UploadFiles(){}

    /**
     * validate client input.
     * @param input - string that the costumer entered.
     * @return - -1 if the string is invalid, 0 o.w.
     */
    int checkInput(string input) {
        string str2 = ".csv";
        if(!(strstr(input.c_str(), str2.c_str()))) {
            return -1;
        }
        return 0;
    }


    /**
     * execute command1.
     */
    virtual void execute() {
        string trainStr = "Please upload your local train CSV file.\n";
        string testStr = "Please upload your local test CSV file.\n";
        string complete = "Upload complete.\n";
        string invalid = "invalid input\n";
        //write trainStr to the client
        dio->write(trainStr);
        string input1;
        int check;
        input1 = dio->read();
        check = checkInput(input1);
        if(check != 0) {
            dio->write(invalid);
            return;
        } else {
            //upload to struct the file name
            dio->write(complete);
        }
        dio->write(testStr);
        string input2;
        input2 = dio->read();
        check = checkInput(input1);
        if(check != 0) {
            dio->write(invalid);
            return;
        } else {
            //upload to struct the file name
            dio->write(complete);
        }
    }

};

class ClassifyData : Command {
public:

    ClassifyData(DeafultIO* dio) : Command(dio) {
        this->description = "3. classify data\n";
    }

    /**
     * destructor.
     */
    virtual ~ClassifyData(){};

    virtual void execute() {

    }



};

#endif //MULTCLIENTREP_COMMANDS_H



