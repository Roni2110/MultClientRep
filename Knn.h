//
// Created by danas on 07/12/2022.
//

#ifndef KNNREP_KNN_H
#define KNNREP_KNN_H
#include "DistanceClass.h"
#include <string>
using namespace std;

class Knn {

private:
    int k;
    string message;
    string disName;
    string file;
    string testFile;
    vector<double> vecInput;
    vector<vector<double>> doubleVec;
    vector<string> stringVec;
    vector<string> finishStrVec;
    vector<vector<pair<double,string>>> pairsVec;

public:
    Knn(int k, string disName, vector<double> v1);
    Knn(int k, string disName, string testFile, string trainFile);
    void uploadFiles(string stringPath, int &flag);
    vector<vector<double>> getVectorsTest(string PathTest);
    void classifyData(string pathFile, vector<vector<double>> vecToTest, int &flag);
    void pushingToPairs(vector<double> d1, vector<string> s1, int index);
    void sortingByDistance(vector<pair<double,string>> resVec, int index);
    void getSignificant(vector<pair<double, string>> sortVec, int index);
    string getMessage();
    vector<string> getResVec();
};
#endif //KNNREP_KNN_H
