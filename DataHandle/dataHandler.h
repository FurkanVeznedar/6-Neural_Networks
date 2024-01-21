#ifndef _DATAHANDLER_H_
#define _DATAHANDLER_H_

#include <fstream>
#include "stdint.h"
#include <vector>
#include "data.h"
#include <string>
#include <map>
#include <unordered_set>
#include <random>

class dataHandler {
private:
    std::vector<data *> * dataarray; // all of the data (pre-split)
    std::vector<data *> * trainingdata;
    std::vector<data *> * testdata;
    std::vector<data *> * validationdata;

    int numclasses;
    double featurevectorsize;
    std::map<uint8_t, int> classfromint;
    std::map<std::string, int> classfromstring;

    const double TRAIN_SET_PERCENT = 0.75;
    const double TEST_SET_PERCENT = 0.20;
    const double VALIDATION_PERCENT = 0.05;

public:
    dataHandler();
    ~dataHandler();

    void readCSV(std::string path, std::string delimiter);
    void readFeatureVector(std::string path);
    void readFeatureLabels(std::string path);
    void splitData();
    void countClasses();
    void normalize();
    void print();

    uint32_t converttolittleendian(const unsigned char* bytes);

    int getClassCounts();
    int getDataArraySize();
    int getTrainingDataSize();
    int getTestDataSize();
    int getValidationSize();
    
    std::vector<data *> *getTrainingData();
    std::vector<data *> *getTestData();
    std::vector<data *> *getValidationData();
    std::map<uint8_t, int> getClassMap();

    std::random_device rd;
};

#endif // _DATAHANDLER_H_