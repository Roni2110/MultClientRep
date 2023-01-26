#include "CLI.h"


/***
* constructor.
 * @param dio - socketIO.
 */
CLI::CLI(DeafultIO* dio) {
    this->dio = dio;
}

/**
 * a function that initializing the costumer information struct, and printing the menu to the user.
 * if the user press 1 - go to upload files.
 * if the user press 2 - changing the neighbors and distance.
 * if the user press 3 - classifying the file uploaded according to different distances.
 * if the user press 4 - displaying results on the screen.
 * if the user press 5 - writing results to a file given.
 * if the user press 8 - exit the program.
 * in any other cases - sending invalid input.
 */
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
        if(chooseOp == -1){
            continue;
        }
        if(chooseOp == 8) {
            chooseOp = 6;
        }
        commands.at(chooseOp - 1)->execute();
    }
}

/**
 * destructor.
 */
CLI::~CLI() = default;

