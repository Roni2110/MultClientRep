
#include "Knn.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>

using namespace std;

/**
 * constructor.
 */
Knn::Knn(int k, string disName, string testPath, string trainPath) {
    this->k = k;
    this->disName = disName;
    this->testFile = testPath;
    this->file = trainPath;
}

vector<vector<double>> Knn::getVectorsTest(string pathTest) {
    string tempByLine, tempByComma;
    vector<vector<double>> vecOfVec;
    //file pointer
    fstream fin;
    //open an existing file
    fin.open(pathTest);
    if(!fin.is_open()) {
        cout << "Cant open file" << endl;
        exit(0);
    }
    int i = 0;
    //getting data from file into two vectors
    while(getline(fin, tempByLine)) {
        stringstream ss(tempByLine);
        vector<double> v1 = vecOfVec.at(i);
        while (getline(ss, tempByComma, ',')) {
            v1.push_back(stod(tempByComma));
        }
        i++;
    }
    return vecOfVec;
}

void Knn::classifyData(string pathFile, vector<vector<double>> vecToTest, int &flag) {
    double res;
    int i = 0;
    int sizeTest = vecToTest.size();
    int sizeTrain;
    vector<vector<double>> resVec;
    string tempByLine, tempByComma;
    //file pointer
    fstream fin;
    //open an existing file
    fin.open(pathFile);
    if(!fin.is_open()) {
        cout << "Cant open file" << endl;
        exit(0);
    }
    while(getline(fin, tempByLine )) {
        stringstream  ss(tempByLine);
        vector<double> temp;
        while (getline(ss, tempByComma, ',')) {
            if(isalpha(tempByComma[0])) {
                stringVec.push_back(tempByComma);
            } else {
                resVec.at(i).push_back(stod(tempByComma));
            }
        }
        i++;
    }
    fin.close();
    sizeTrain = resVec.size();
    //in case one of the vectors in the trainFile in invalid.
    if(stringVec.size() != sizeTrain) {
        flag = -1;
        return;
    }
    for(int i = 0; i < sizeTrain; i++) {
        for(int j = 0; j < sizeTest; j++) {
            //in case the vectors to test are not in the same size.
            if (resVec.at(i).size() != vecToTest.at(j).size()) {
                flag = -1;
                return;
            }
            //calling the distance method according to disName
            if (this->disName == "MAN") {
                res = DistanceClass::getManDis(resVec.at(i), vecToTest.at(j));
                doubleVec.at(i).push_back(res);
            } else if (this->disName == "AUC") {
                res = DistanceClass::getEucDis(resVec.at(i), vecToTest.at(j));
                doubleVec.at(i).push_back(res);
            } else if (this->disName == "CHB") {
                res = DistanceClass::getChebDis(resVec.at(i), vecToTest.at(j));
                doubleVec.at(i).push_back(res);
            } else if (this->disName == "CAN") {
                res = DistanceClass::getCanDis(resVec.at(i), vecToTest.at(j));
                if (res == -1) {
                    this->message = "invalid input";
                    flag = -1;
                    return;
                }
                doubleVec.at(i).push_back(res);
            } else {
                res = DistanceClass::getMinkDis(resVec.at(i), vecToTest.at(j));
                doubleVec.at(i).push_back(res);
            }
        }
        //in case number of neighbours is bigger than distances.
        if(this->k > doubleVec.at(i).size()) {
            this->message = "invalid input";
            flag = -1;
            return;
        }
        pushingToPairs(doubleVec.at(i),stringVec,i);
    }
}

/**
 * pushing the vectors (double, string) to one pair vector.
 * @param d1 - double vector with the distances.
 * @param s1 - string vector with the names.
 */
void Knn::pushingToPairs(vector<double> d1, vector<std::string> s1, int j) {
    for(int i = 0; i < d1.size(); i++) {
        pairsVec.at(j).emplace_back(d1.at(i), s1.at(i));
    }
    sortingByDistance(pairsVec.at(j), j);
}

/**
 * sorting the pair vector by distance in ascending order.
 * @param vector - pair vector with distance and name.
 */
void Knn::sortingByDistance(vector<pair<double,string>> vector, int index){
    sort(vector.begin(), vector.end());
    getSignificant(vector,index);
}

/**
 * get the significant label from the k nearest neighbors.
 * @param pairs - sorted pair vector with distance and name.
 */
void Knn::getSignificant(vector<pair<double, string>> pairs, int index) {
    std::map<string, int> map;
    int i;
    int temp = 0;
    int max = 0;
    string res;
    vector<string> strVec;
    for(i = 0; i < this->k; i++) {
        map[pairs[i].second]++;
        temp = map[pairs[i].second];
        if (max <= temp) {
            max = temp;
            strVec.at(index) = pairs[i].second;
        }
    }
    this->finishStrVec.at(i) = strVec.at(index);
}

vector<string> Knn::getResVec() {
    return this->finishStrVec;
}

