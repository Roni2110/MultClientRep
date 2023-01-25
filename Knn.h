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
    string disName;
    vector<vector<double>> trainVector;
    vector<vector<double>> testVector;
    vector<string> stringVec;
    vector<string> finishStrVec;
    vector<pair<double,string>> pairsVec;

public:
    Knn(int k, string disName, vector<vector<double>> train, vector<vector<double>> test,vector<string> str);
    void classifyData(int &flag);
    void pushingToPairs(vector<double> d1, vector<string> s1);
    void sortingByDistance(vector<pair<double,string>> resVec);
    void getSignificant(vector<pair<double, string>> sortVec);
    vector<string> getResVec();
};
#endif //KNNREP_KNN_H
