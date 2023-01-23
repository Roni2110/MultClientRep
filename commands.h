//
// Created by danas on 1/19/2023.
//

#ifndef MULTCLIENTREP_COMMANDS_H
#define MULTCLIENTREP_COMMANDS_H


#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include "Knn.h"


using namespace std;

struct info{
    string train;
    string test;
    int k;
    string DIS;
    vector<string> results;
};

class DeafultIO{
public:
    virtual string read()= 0;
    virtual void write(string text)=0;
    virtual ~DeafultIO()= default;
};


class Command{
protected:
    DeafultIO* dio;
    string description;
    info* information;
public:
    Command(DeafultIO* dio, struct info* info1): dio(dio), information(info1){}
    virtual void execute()=0;
    virtual ~Command()= default;
    virtual void print(){
        dio->write(description);
    }
};
/**
 * first command.
 */
class UploadFiles : public Command {
public:

    UploadFiles(DeafultIO* dio, struct info* info) : Command(dio, info){
        this->description = "1. upload an unclassified csv data files\n";
    }

    /**
     * destructor.
     */
    ~UploadFiles() override= default;;

    /**
     * validate client input.
     * @param input - string that the costumer entered.
     * @return - -1 if the string is invalid, 0 o.w.
     */
    static int checkInput(string input) {
        string str2 = ".csv";
        if(!(strstr(input.c_str(), str2.c_str()))) {
            return -1;
        }
        return 0;
    }


    /**
     * execute command1.
     */
    void execute() override {
        string trainStr = "Please upload your local train CSV file.\n";
        string testStr = "Please upload your local test CSV file.\n";
        string complete = "Upload complete.\n";
        string invalid = "invalid input\n";
        //write trainStr to the client
        cout<<trainStr<<endl;
        dio->write(trainStr);
        string input1;
        int check;
        input1 = dio->read();
        check = checkInput(input1);
        if(check == -1) {
            dio->write(invalid);
            return;
        } else {
            //upload to struct the file name
            this->information->train = input1;
            dio->write(complete);
        }
        dio->write(testStr);
        string input2;
        input2 = dio->read();
        check = checkInput(input1);
        if(check == -1) {
            dio->write(invalid);
            return;
        } else {
            //upload to struct the file name
            this->information->test = input2;
            dio->write(complete);
        }
    }
};

/**
 * command 2.
 */
class AlgorithmSetting: public Command{
public:
    AlgorithmSetting(DeafultIO* dio, struct info* info): Command(dio, info) {
        this->description = "2. algorithm settings\n";
        this->information->k = 5;
        this->information->DIS = "AUC";
    }
    /**
 * destructor.
 */
    ~AlgorithmSetting() override= default;;
    void execute() override {
        int check = 0;
        int currentK = information->k;
        string invalidK = "invalid value for K";
        string invalidDis = "invalid value for metric";
        string invalidInput = "invalid input";
        string kToStr = to_string(currentK);
        string currentDistance = information->DIS;
        string currentInfo = "The current KNN parameters are: k = ";
        dio->write(currentInfo + " " + kToStr + " " + "distance metric = " + currentDistance);
        string input = dio->read();
        string str;
        int intNum;
        //check that there is an int.
        stringstream ss(input);
        if (ss >> intNum) {
            ss << intNum;
        }
        if (!ss > 0) {
            check = 1;
        }
        //check that there is a string.
        ss.clear();
        if (ss >> str) {
            ss << str;
        }
        if ((ss.str() != "AUC") && (ss.str() != "MAN") && (ss.str() != "CHB")
        && (ss.str() != "CAN") && (ss.str() != "MIN")) {
            if(check != 1){
                check = 2;
            } else {
                check = 3;
            }
        }
        if (!ss.eof()) {
            dio->write(invalidInput);
            return;
        }
        if(check == 1){
            dio->write(invalidK);
            return;
        }
        if(check == 2){
            dio->write(invalidDis);
            return;
        }
        if(check == 3){
            dio->write(invalidK +"\n"+ invalidInput);
            return;
        }
        this->information->k = intNum;
        this->information->DIS = str;
        string valid = "valid";
        dio->write(valid);
    }
};

/**
 * command 3.
 */
class ClassifyData : public Command {
public:

    ClassifyData(DeafultIO* dio, struct info* info) : Command(dio, info) {
        this->description = "3. classify data\n";
    }

    /**
     * destructor.
     */
    ~ClassifyData() override= default;;

    void execute() override {
        string notUpload = "please upload data\n";
        string complete = "classifying data complete\n";
        string invalid = "invalid input\n";
        int flag = 0;
        vector<string> v1;
        vector<vector<double>> resTest;
        if(this->information->test.empty() || this->information->train.empty()) {
            dio->write(notUpload);
            return;
        }
        Knn* knn = new Knn(this->information->k, this->information->DIS, this->information->test,
                           this->information->train);
        resTest = knn->getVectorsTest(this->information->test);
        knn->classifyData(this->information->train, resTest, flag);
        if(flag == -1) {
            dio->write(invalid);
            delete knn;
            return;
        }
        dio->write(complete);
        this->information->results = knn->getResVec();
        delete knn;
    }
};

/**
 * command 4.
 */
class DisplayResult : public Command {
public:
    DisplayResult(DeafultIO* dio, struct info* info) : Command(dio, info) {
        this->description = "4. display results\n";
    }

    /**
     * destructor.
     */
    ~DisplayResult() override= default;;
    void execute() override {
        string invalid1 = "please upload data.";
        string invalid2 = "please classify data.";
        string done = "Done.";
        if(this->information->train.empty() || this->information->test.empty()) {
            dio->write(invalid1);
            return;
        }
        if(this->information->results.empty()) {
            dio->write(invalid2);
            return;
        }
        int size = this->information->results.size();
        for(int i = 0; i < size; i++) {
            dio->write(this->information->results.at(i));
        }
        dio->write(done);
    }

};

class DownloadResult : public Command {
public:
    DownloadResult(DeafultIO* dio, struct info* info) : Command(dio, info) {
        this->description = "5. download results\n";
    }

    /**
 * destructor.
 */
    ~DownloadResult() override= default;;
    void execute() override {
        string invalid1 = "please upload data.\n";
        string invalid2 = "please classify data.\n";
        string done = "Done.\n";
        if(this->information->train.empty() || this->information->test.empty()) {
            dio->write(invalid1);
            return;
        }
        if(this->information->results.empty()) {
            dio->write(invalid2);
            return;
        }
        int size = this->information->results.size();
        for(int i = 0; i < size; i++) {
            dio->write(this->information->results.at(i));
        }
        dio->write(done);
    }
};

/**
 * command 8.
 */
class Exit: public Command{
public:
    Exit(DeafultIO* dio, struct info* info) : Command(dio, info) {
        this->description = "8. exit\n";
    }
    ~Exit() override= default;;

    void execute() override{
        //delete all the data
        delete information;
        //close the CLI
        string close_socket = "close";
        dio->write(close_socket);
    }

};

#endif //MULTCLIENTREP_COMMANDS_H



