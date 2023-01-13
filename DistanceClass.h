//
// Created by danas on 07/12/2022.
//

#ifndef KNNREP_DISTANCECLASS_H
#define KNNREP_DISTANCECLASS_H


#include <vector>

class DistanceClass{
public:
    static double getEucDis(std::vector <double> v1, std::vector <double> v2);
    static double getCanDis(std::vector <double> v1, std::vector <double> v2);
    static double getChebDis(std::vector<double> v1, std::vector<double> v2);
    static double getManDis(std::vector<double> v1, std::vector<double> v2);
    static double getMinkDis(std::vector<double> v1, std::vector<double> v2);
};


#endif //KNNREP_DISTANCECLASS_H
