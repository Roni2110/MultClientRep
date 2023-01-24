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
    vector<double> doubleVec;
    vector<string> stringVec;
    vector<string> finishStrVec;
    vector<pair<double,string>> pairsVec;

public:
    Knn(int k, string disName, string testFile, string trainFile);
    vector<vector<double>> getVectorsTest(string PathTest);
    void classifyData(string pathFile, vector<vector<double>> vecToTest, int &flag);
    void pushingToPairs(vector<double> d1, vector<string> s1);
    void sortingByDistance(vector<pair<double,string>> resVec);
    void getSignificant(vector<pair<double, string>> sortVec);
    vector<string> getResVec();
};
#endif //KNNREP_KNN_H
