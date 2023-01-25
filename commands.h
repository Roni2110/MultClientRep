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
#include <fstream>
#include "Knn.h"


using namespace std;

struct info{
    //string train;
    //string test;
    vector<string> strVec;
    vector<vector<double>> trainVec;
    vector<vector<double>> testVec;
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
//    static int checkInput(string input) {
//        string str2 = ".csv";
//        if(!(strstr(input.c_str(), str2.c_str()))) {
//            return -1;
//        }
//        return 0;
//    }
//
//    static int checkIfOpen(string path) {
//        //file pointer
//        fstream fin;
//        //open an existing file
//        fin.open(path);
//        if(!fin.is_open()) {
//            cout << "Cant open file" << endl;
//            return -1;
//        }
//        return 0;
//    }

    void uploadTrain(string line) {
        vector<vector<double>> resVec;
        string tempByComma;
        stringstream  ss(line);
        vector<double> temp;
        while (getline(ss, tempByComma, ',')) {
            if(isalpha(tempByComma[0])) {
                this->information->strVec.push_back(tempByComma);
            } else {
                temp.push_back(stod(tempByComma));
            }
        }
        this->information->trainVec.push_back(temp);
    }

    void uploadTest(string line) {
        vector<vector<double>> resVec;
        string tempByComma;
        stringstream  ss(line);
        vector<double> temp;
        while (getline(ss, tempByComma, ',')) {
            temp.push_back(stod(tempByComma));
        }
        this->information->testVec.push_back(temp);
    }




    /**
     * execute command1.
     */
    void execute() override {
        string trainStr = "Please upload your local train CSV file.";
        string testStr = "Please upload your local test CSV file.";
        string complete = "Upload complete.";
        string invalid = "invalid input";
        //write trainStr to the client
        dio->write(trainStr);
        //upload train file
        string input;
        input = dio->read();
        if(input == "stop"){
            return;
        }
        while(input != "finish") {
            uploadTrain(input);
            input = dio->read();
        }
        dio->write(complete);
        //write testStr to the client
        dio->write(testStr);
        string input2;
        input2 = dio->read();
        if(input2 == "stop"){
            return;
        }
        while(input2 != "finish") {
            uploadTest(input2);
            input2 = dio->read();
        }
        dio->write(complete);
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
        string currentInfo = "The current KNN parameters are: k =";
        dio->write(currentInfo + " " + kToStr + " " + "distance metric = " + currentDistance);
        string input = dio->read();
        if(input.empty()) {
            return;
        }
        string str;
        int intNum;
        //check that there is an int.
        stringstream ss(input);
        if (ss >> intNum) {
            ss << intNum;
        }
        if (intNum <= 0) {
            check = 1;
        }
        //check that there is a string.
        ss.clear();
        if (ss >> str) {
            ss << str;
        }
        if ((str != "AUC") && (str != "MAN") && (str != "CHB")
        && (str != "CAN") && (str != "MIN")) {
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
            dio->write(invalidK +"\n"+ invalidDis);
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
        string notUpload = "please upload data";
        string complete = "classifying data complete";
        string invalid = "invalid input";
        string wait = "wait";
        int flag = 0;
        vector<string> v1;
        vector<vector<double>> resTest;
        if(this->information->trainVec.empty() || this->information->testVec.empty()) {
            dio->write(notUpload);
            return;
        }
        Knn* knn = new Knn(this->information->k, this->information->DIS, this->information->trainVec,
                           this->information->testVec,this->information->strVec);
        knn->classifyData(flag);
        if(flag == -1) {
            delete knn;
            dio->write(invalid);
            return;
        }
        this->information->results = knn->getResVec();
        delete knn;
        dio->write(complete);
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
        if(this->information->trainVec.empty() || this->information->testVec.empty()) {
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
        string invalid1 = "please upload data.";
        string invalid2 = "please classify data.";
        string done = "Done.";
        if(this->information->trainVec.empty() || this->information->testVec.empty()) {
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
        //close the CLI
        string close_socket = "close";
        dio->write(close_socket);
    }

};

#endif //MULTCLIENTREP_COMMANDS_H



