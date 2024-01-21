#include "knn.h"
#include <cmath>
#include <limits>
#include <map>
#include "stdint.h"
#include "../DataHandle/inputdata.h"

knn::knn(int val) {
    k = val;
}

knn::knn() {

}

knn::~knn() {

}

// O(N^2) if K ~ N
// if K = 2 O(~N)
// O(NlogN) sorted algorithm

void knn::findKNearest(data *querypoint) {
    neighbours = new std::vector<data *>;
    double min = std::numeric_limits<double>::max();
    double previousmin = min;
    int index = 0;
    for(int i = 0; i < k; i++) {
        if(i == 0) {
            for(int j = 0; j < trainingdata->size(); j++) {
                double distance = calculateDistance(querypoint, trainingdata->at(j));
                trainingdata->at(j)->setDistance(distance);
                if(distance < min) {
                    min = distance;
                    index = j;
                }
            }
            neighbours->push_back(trainingdata->at(index));
            previousmin = min;
            min = std::numeric_limits<double>::max();
        } else {
            for(int j = 0; j < trainingdata->size(); j++) {
                double distance = trainingdata->at(j)->getDistance();
                if(distance > previousmin && distance < min) {
                    min = distance;
                    index = j;
                }
            }
            neighbours->push_back(trainingdata->at(index));
            previousmin = min;
            min = std::numeric_limits<double>::max();
        }
    }
}

void knn::setK(int val) {
    k = val;
}

int knn::predict() {
    std::map<uint8_t, int> classfreq;
    for(int i = 0; i < neighbours->size(); i++) {
        if(classfreq.find(neighbours->at(i)->getLabel()) == classfreq.end()) {
            classfreq[neighbours->at(i)->getLabel()] = 1;
        } else {
            classfreq[neighbours->at(i)->getLabel()]++;
        }
    }

    int best = 0;
    int max = 0;
    for(auto kv: classfreq) {
        if(kv.second > max) {
            max = kv.second;
            best = kv.first;
        }
    }

    neighbours->clear();
    return best;
}

double knn::calculateDistance(data *querypoint, data *input) {
    double distance = 0.0;
    if(querypoint->getFeatureVectorSize() != input->getFeatureVectorSize()) {
        printf("Error Vector Size Mismatch.\n");
        exit(1);
    }

#ifdef EUCLID
    for(unsigned i = 0; i < querypoint->getFeatureVectorSize(); i++) {
        distance += pow(querypoint->getFeatureVector()->at(i) - input->getFeatureVector()->at(i), 2);
    }
    distance = sqrt(distance);
#elif defined MANHATTEN
    for(unsigned i = 0; i < querypoint->getFeatureVectorSize(); i++) {
        distance += querypoint->getFeatureVector()->at(i) - input->getFeatureVector()->at(i);
    }
    distance = sqrt(distance);
#endif

    return distance;
}

double knn::validatePerformance() {
    double currentperformance = 0;
    int count = 0;
    int dataindex = 0;
    for(data *querypoint: *validationdata) {
        findKNearest(querypoint);
        int prediction = predict();
        printf("%d -> %d \n", prediction, querypoint->getLabel());
        if(prediction == querypoint->getLabel()) {
            count++;
        }
        dataindex++;
        printf("Current Performance = %.3f %%\n", ((double)count * 100.0) / ((double)dataindex));
    }
    currentperformance = ((double)count * 100.0) / ((double)validationdata->size());
    printf("Validation Performance for K = %d = %.3f %%\n", k, currentperformance);
    return currentperformance;
}

double knn::testPerformance() {
    double currentperformance = 0;
    int count = 0;
    for(data *querypoint: *testdata) {
        findKNearest(querypoint);
        int prediction = predict();
        if(prediction == querypoint->getLabel()) {
            count++;
        }
    }
    currentperformance = ((double)count * 100.0) / ((double)testdata->size());
    printf("Tested Performance = %.3f %%\n", currentperformance);
    return currentperformance;
}
