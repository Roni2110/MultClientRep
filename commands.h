//
// Created by danas on 1/19/2023.
//

#ifndef MULTCLIENTREP_COMMANDS_H
#define MULTCLIENTREP_COMMANDS_H


#include <iostream>
#include <string.h>
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
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DeafultIO(){}

};


class Command{
protected:
    DeafultIO* dio;
    string description;
    info* info;
public:
    Command(DeafultIO* dio, struct info* info): dio(dio), info(info){}
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

    UploadFiles(DeafultIO* dio, struct info* info) : Command(dio, info){
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
        if(check == -1) {
            dio->write(invalid);
            return;
        } else {
            //upload to struct the file name
            this->info->train = input1;
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
            this->info->test = input2;
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
        this->info->k = 5;
        this->info->DIS = "AUC";
    }
    void execute() {
        int currentK = info->k;
        string currentDistance = info->DIS;
        string currentInfo = "The current KNN parameters are: k = ";
        dio->write(currentInfo);
        dio->write(currentK);
        dio->write(", distance metric = ");
        dio->write(currentDistance);
        string input = dio->read();
        if(input.length() == 0){
            return;
        }
        else {
            string str;
            int intNum;
            //check that there is an int.
            stringstream ss(input);
            if (ss >> intNum) {
                ss << intNum;
            }
            if (!ss > 0) {
                string invalidK = "invalid value for K";
                dio->write(invalidK);
            }
            //check that there is a string.
            ss.clear();
            if (ss >> str) {
                ss << str;
            }
            if ((ss.str() != "AUC") && (ss.str() != "MAN") && (ss.str() != "CHB")
                && (ss.str() != "CAN") && (ss.str() != "MIN")) {
                string invalidDis = "invalid value for metric";
                dio->write(invalidDis);
                return;
            }
            if (!ss.eof()) {
                string invalidInput = "invalid input";
                dio->write(invalidInput);
                return;
            }
            this->info->k = intNum;
            this->info->DIS = str;
            return;
        }
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
    virtual ~ClassifyData(){};

    virtual void execute() {
        string notUpload = "please upload data\n";
        string complete = "classifying data complete\n";
        string invalid = "invalid input\n";
        int flag = 0;
        int i = 1;
        vector<string> v1;
        vector<vector<double>> resTest;
        if(this->info->test.empty() || this->info->train.empty()) {
            dio->write(notUpload);
            return;
        }
        Knn* knn = new Knn(this->info->k, this->info->DIS, this->info->test, this->info->train);
        resTest = knn->getVectorsTest(this->info->test);
        knn->classifyData(this->info->train, resTest, flag);
        if(flag == -1) {
            dio->write(invalid);
            delete knn;
            return;
        }
        dio->write(complete);
        this->info->results = knn->getResVec();
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
    virtual ~DisplayResult(){};
    virtual void execute() {
        string invalid1 = "please upload data.\n";
        string invalid2 = "please classify data.\n";
        string done = "Done.\n";
        if(this->info->train.empty() || this->info->test.empty()) {
            dio->write(invalid1);
            return;
        }
        if(this->info->results.empty()) {
            dio->write(invalid2);
            return;
        }
        int size = this->info->results.size();
        for(int i = 0; i < size; i++) {
            dio->write(this->info->results.at(i));
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
    virtual ~DownloadResult(){};
    virtual void execute() {
        string invalid1 = "please upload data.\n";
        string invalid2 = "please classify data.\n";
        string done = "Done.\n";
        if(this->info->train.empty() || this->info->test.empty()) {
            dio->write(invalid1);
            return;
        }
        if(this->info->results.empty()) {
            dio->write(invalid2);
            return;
        }
        int size = this->info->results.size();
        for(int i = 0; i < size; i++) {
            dio->write(this->info->results.at(i));
        }
    }
};

#endif //MULTCLIENTREP_COMMANDS_H



