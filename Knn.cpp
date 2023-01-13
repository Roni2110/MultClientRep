
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
 * @param k - the k nearest neighbors.
 * @param disName - the distance we want to calculate by.
 * @param v1 - input from the user.
 */
Knn::Knn(int k, string disName, vector<double> v1) {
    this->k = k;
    this->disName = disName;
    this->vecInput = v1;
    this->message = "";
}

/**
 * uploadFiles - getting all data from path we get from the user.
 * each raw we put in a double vector - and calculating the distance from vector input.
 * every raw - in his last column has the name of the vector - putting it in a string vector.
 * lastly, we have two vectors - on with the distances, and one with the name.
 * @param stringPath - the path to the data we want to upload.
 */
void Knn::uploadFiles(std::string stringPath, int &flag) {
    double res;
    string tempByLine, tempByComma;
    //file pointer
    fstream fin;
    //open an existing file
    fin.open(stringPath);
    if(!fin.is_open()) {
        cout << "Cant open file" << endl;
        exit(0);
    }
    //getting data from file into two vectors
    while(getline(fin, tempByLine)) {
        stringstream ss(tempByLine);
        vector<double> temp;
        while(getline(ss, tempByComma, ',')) {
            if(isalpha(tempByComma[0])) {
                stringVec.push_back(tempByComma);
            } else {
                temp.push_back(stod(tempByComma));
            }
        }
        //checking if the vector in data in the same size a vector input
        if(temp.size() != vecInput.size()) {
            this->message = "invalid input";
            flag = -1;
            return;
        }
        //calling the distance method according to disName
        if(this->disName == "MAN") {
            res = DistanceClass::getManDis(temp,vecInput);
            doubleVec.push_back(res);
        } else if(this->disName == "AUC") {
            res = DistanceClass::getEucDis(temp, vecInput);
            doubleVec.push_back(res);
        } else if(this->disName == "CHB") {
            res = DistanceClass::getChebDis(temp, vecInput);
            doubleVec.push_back(res);
        } else if(this->disName == "CAN") {
            res = DistanceClass::getCanDis(temp, vecInput);
            if(res == -1) {
                this->message = "invalid input";
                flag = -1;
                return;
            }
            doubleVec.push_back(res);
        } else {
            res = DistanceClass::getMinkDis(temp, vecInput);
            doubleVec.push_back(res);
        }
        //check that the distance vector is at the same size as the string vector - if not the file is not proper
        //,so it exit the code.
        if(doubleVec.size() != stringVec.size()) {
            this->message = "invalid input";
            flag = -1;
            return;
        }
    }
    fin.close();
    //checking if k given in bigger then vectors
    if(this->k > doubleVec.size()) {
        this->message = "invalid input";
        flag = -1;
        return;
    }
    pushingToPairs(doubleVec, stringVec);
}

/**
 * pushing the vectors (double, string) to one pair vector.
 * @param d1 - double vector with the distances.
 * @param s1 - string vector with the names.
 */
void Knn::pushingToPairs(vector<double> d1, vector<std::string> s1) {
    for(int i = 0; i < d1.size(); i++) {
        resVec.emplace_back(d1.at(i), s1.at(i));
    }
    sortingByDistance(resVec);
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
    for(i = 0; i < this->k; i++) {
        map[pairs[i].second]++;
        temp = map[pairs[i].second];
        if (max <= temp) {
            max = temp;
            res = pairs[i].second;
        }
    }
    this->message = res;
}

/**
 * get the message to client.
 * @return the string that knn calculate and the client will print.
 */
string Knn::getMessage() {
    return this->message;
}

