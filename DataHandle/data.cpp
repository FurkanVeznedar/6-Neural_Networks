#include "data.h"

data::data() {
    featurevector = new std::vector<uint8_t>;
    classvector = new std::vector<int>;
}

data::~data() {
    delete featurevector;
    delete classvector;
}

void data::setFeatureVector(std::vector<uint8_t> *vect) {
    featurevector = vect;
}

void data::appendToFeatureVector(uint8_t val) {
    featurevector->push_back(val);
}

void data::setNormalizedFeatureVector(std::vector<double> * vect) {
    normalizedfeaturevector = vect;
}

void data::appendToFeatureVector(double val) {
    normalizedfeaturevector->push_back(val);
}

void data::setLabel(uint8_t val) {
    label = val;
}

void data::setEnumeratedLabel(uint8_t val) {
    enumlabel = val;
}

void data::setDistance(double val) {
    distance = val;
}

void data::setClassVector(int count) {
    for(int i = 0; i < count; i++) {
        if(i == label) {
            classvector->push_back(1);
        } else {
            classvector->push_back(0);
        }
    }
}

void data::printVector() {
  printf("[ ");
  for(uint8_t val : *featurevector) {
    printf("%u ", val);
  }
  printf("]\n");
}

void data::printNormalizedVector() {
  printf("[ ");
  for(auto val : *normalizedfeaturevector) {
    printf("%.2f ", val);
  }
  printf("]\n");
}

double data::getDistance() {
    return distance;
}

int data::getFeatureVectorSize() {
    return featurevector->size();
}

uint8_t data::getLabel() {
    return label;
}

uint8_t data::getEnumeratedLabel() {
    return enumlabel;
}

std::vector<uint8_t> * data::getFeatureVector() {
    return featurevector;
}

std::vector<double> * data::getNormalizedFeatureVector() {
    return normalizedfeaturevector;
}

std::vector<int> data::getClassVector() {
    return *classvector;
}