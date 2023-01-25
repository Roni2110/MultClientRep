
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
Knn::Knn(int k, string disName, vector<vector<double>> train, vector<vector<double>> test,vector<string> str) {
    this->k = k;
    this->disName = disName;
    this->stringVec = str;
    this->trainVector = train;
    this->testVector = test;
}
//
//vector<vector<double>> Knn::getVectorsTest(string pathTest) {
//    string tempByLine, tempByComma;
//    vector<vector<double>> vecOfVec;
//    //file pointer
//    fstream fin;
//    //open an existing file
//    fin.open(pathTest);
//    if(!fin.is_open()) {
//        cout << "Cant open file" << endl;
//       exit(0);
//   }
//    //getting data from file into two vectors
//    while(getline(fin, tempByLine)) {
//        vector<double> v1;
//        stringstream ss(tempByLine);
//        while (getline(ss, tempByComma, ',')) {
//            v1.push_back(stod(tempByComma));
//        }
//        vecOfVec.push_back(v1);
//    }
//    return vecOfVec;
//}

void Knn::classifyData(int &flag) {
    double res;
    int sizeTest = this->testVector.size();
    int sizeTrain = this->trainVector.size();
    //in case one of the vectors in the trainFile in invalid.
    if(stringVec.size() != sizeTrain) {
        flag = -1;
        return;
    }
    //in case number of neighbours is bigger than distances.
    if(this->k > sizeTrain) {
        this->k = sizeTrain;
    }
    for(int i = 0; i < sizeTest; i++) {
        vector<double>temp;
        for(int j = 0; j < sizeTrain; j++) {
            //in case the vectors to test are not in the same size.
            if (trainVector.at(j).size() != testVector.at(i).size()) {
                flag = -1;
                return;
            }
            //calling the distance method according to disName
            if (this->disName == "MAN") {
                res = DistanceClass::getManDis(trainVector.at(j), testVector.at(i));
                temp.push_back(res);
            } else if (this->disName == "AUC") {
                res = DistanceClass::getEucDis(trainVector.at(j), testVector.at(i));
                temp.push_back(res);
            } else if (this->disName == "CHB") {
                res = DistanceClass::getChebDis(trainVector.at(j), testVector.at(i));
                temp.push_back(res);
            } else if (this->disName == "CAN") {
                res = DistanceClass::getCanDis(trainVector.at(j), testVector.at(i));
                if (res == -1) {
                    flag = -1;
                    return;
                }
                temp.push_back(res);
            } else {
                res = DistanceClass::getMinkDis(trainVector.at(j), testVector.at(i));
                temp.push_back(res);
            }
        }
        pushingToPairs(temp,stringVec);
    }
}

/**
 * pushing the vectors (double, string) to one pair vector.
 * @param d1 - double vector with the distances.
 * @param s1 - string vector with the names.
 */
void Knn::pushingToPairs(vector<double> d1, vector<std::string> s1) {
    for(int i = 0; i < d1.size(); i++) {
        pairsVec.emplace_back(d1.at(i), s1.at(i));
    }
    sortingByDistance(pairsVec);
}

/**
 * sorting the pair vector by distance in ascending order.
 * @param vector - pair vector with distance and name.
 */
void Knn::sortingByDistance(vector<pair<double,string>> vector){
    sort(vector.begin(), vector.end());
    getSignificant(vector);
}

/**
 * get the significant label from the k nearest neighbors.
 * @param pairs - sorted pair vector with distance and name.
 */
void Knn::getSignificant(vector<pair<double, string>> pairs) {
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
            res = pairs[i].second;
        }
    }
    this->finishStrVec.push_back(res);
}

vector<string> Knn::getResVec() {
    return this->finishStrVec;
}

