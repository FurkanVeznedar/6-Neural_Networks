#include "common.h"



void common::setTrainingData(std::vector<data *> *vect) {
    trainingdata = vect;
}

void common::setTestData(std::vector<data *> *vect) {
    testdata = vect;
}

void common::setValidationData(std::vector<data *> *vect) {
    validationdata = vect;
}