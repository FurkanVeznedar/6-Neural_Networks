#ifndef _DATA_H_
#define _DATA_H_

#include <vector>
#include "stdint.h"
#include "stdio.h"

class data {
private:
    std::vector<uint8_t> * featurevector;  // No class at end
    std::vector<double> * normalizedfeaturevector;
    std::vector<int> * classvector;
    uint8_t label;
    uint8_t enumlabel;  // A -> 1, B -> 2
    double distance;

public:
    data();
    ~data();
    void setFeatureVector(std::vector<uint8_t> *);
    void setNormalizedFeatureVector(std::vector<double> *);
    void appendToFeatureVector(uint8_t);
    void appendToFeatureVector(double);
    void setClassVector(int count);
    void setLabel(uint8_t);
    void setEnumeratedLabel(uint8_t val);
    void setDistance(double val);
    void printVector();
    void printNormalizedVector();

    int getFeatureVectorSize();
    uint8_t getLabel();
    uint8_t getEnumeratedLabel();
    double getDistance();
    std::vector<uint8_t> * getFeatureVector();
    std::vector<double> * getNormalizedFeatureVector();
    std::vector<int> getClassVector();
};

#endif // _DATA_H_