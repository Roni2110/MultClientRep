#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "DistanceClass.h"


using namespace std;
int p = 2;


/**
 * this function calculate the euclidean distance between two vectors.
 * @param v1 - the first vector.
 * @param v2 - the second vector.
 */
double DistanceClass::getEucDis(vector<double> v1, vector<double> v2) {
    double sum = 0;
    int power = 2;
    int i;
    double res;
    for (i = 0; i < v1.size(); i++) {
        sum += pow(v1.at(i) - v2.at(i), power);
    }
    res = sqrt(sum);
    return res;
}

/**
 * this function calculate the canberra distance between two vectors.
 * @param v1 - the first vector.
 * @param v2 - the second vector.
 */
double DistanceClass::getCanDis(vector<double> v1, vector<double> v2) {
    double sum = 0;
    int i;
    for (i = 0; i < v1.size(); i++) {
        if (abs(v1.at(i)) + abs(v2.at(i)) == 0) {
            return -1;
        }
        sum += (abs(v1.at(i) - v2.at(i)) / (abs(v1.at(i)) + abs(v2.at(i))));
    }
    return sum;
}

/**
 * this function calculate the chebyshev distance between two vectors.
 * @param v1 - the first vector.
 * @param v2 - the second vector.
 */
double DistanceClass::getChebDis(vector<double> v1, vector<double> v2) {
    int i;
    double max = abs(v1.at(0) - v2.at(0));
    for (i = 1; i < v1.size(); i++) {
        if (abs(v1.at(i) - v2.at(i)) > max) {
            max = abs(v1.at(i) - v2.at(i));
        }
    }
    return max;
}

/**
 * this function calculate the manhattan distance between two vectors.
 * @param v1 - the first vector.
 * @param v2 - the second vector.
 */
double DistanceClass::getManDis(vector<double> v1, vector<double> v2) {
    double sum = 0;
    int i;
    for (i = 0; i < v1.size(); i++) {
        sum += abs((v1.at(i) - v2.at(i)));
    }
    return sum;
}

/**
 * this function calculate the minkowski distance between two vectors.
 * @param v1 - the first vector.
 * @param v2 - the second vector.
 */
double DistanceClass::getMinkDis(vector<double> v1, vector<double> v2) {
    double sum = 0;
    int i;
    double res;
    for (i = 0; i < v1.size(); i++) {
        sum += pow(abs(v1.at(i) - v2.at(i)), p);
    }
    res = pow(sum, (1.0 / p));
    return res;
}