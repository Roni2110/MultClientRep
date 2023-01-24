#include "CLI.h"


//constructor
CLI::CLI(DeafultIO* dio) {
    this->dio = dio;
}

void CLI::start() {
    info my_info;
    string starting = "Welcome to the KNN Classifier server. Please choose an option:\n";
    UploadFiles uploadFiles(this->dio, &my_info);
    commands.push_back(&uploadFiles);
    AlgorithmSetting algorithmSetting(this->dio, &my_info);
    commands.push_back(&algorithmSetting);
    ClassifyData classifyData(this->dio, &my_info);
    commands.push_back(&classifyData);
    DisplayResult displayResult(this->dio, &my_info);
    commands.push_back(&displayResult);
    DownloadResult downloadResult(this->dio, &my_info);
    commands.push_back(&downloadResult);
    Exit exit1(this->dio, &my_info);
    commands.push_back(&exit1);
    int size = commands.size();
    while(true) {
        dio->write(starting);
        for (int i = 0; i < size; i++) {
            commands.at(i)->print();
        }
        int chooseOp = stoi(dio->read());
        if(chooseOp == 8) {
            chooseOp = 6;
        }
        commands.at(chooseOp - 1)->execute();
    }
}

CLI::~CLI() = default;

