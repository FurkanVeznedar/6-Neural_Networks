#include <numeric>
#include "./DataHandle/inputdata.h"
#include "./knn/knn.h"
#include "./kMeans/kmeans.h"
#include "./neuralNetwork/network.h"

void test1() {
    dataHandler *dh = new dataHandler();
    dh->readFeatureVector("../../DataSet/train-images.idx3-ubyte");
    dh->readFeatureLabels("../../DataSet/train-labels.idx1-ubyte");
    dh->splitData();
    dh->countClasses();
}

void test2() {
    dataHandler *dh = new dataHandler();
    dh->readFeatureVector("../../DataSet/train-images.idx3-ubyte");
    dh->readFeatureLabels("../../DataSet/train-labels.idx1-ubyte");
    dh->splitData();
    dh->countClasses();
    knn *knearest = new knn();
    knearest->setTrainingData(dh->getTrainingData());
    knearest->setTestData(dh->getTestData());
    knearest->setValidationData(dh->getValidationData());
    double performance = 0;
    double bestperformance = 0;
    int bestk = 1;
    for(int i = 1; i <= 4; i++) {
        if(i == 1) {
            knearest->setK(i);
            performance = knearest->validatePerformance();
            bestperformance = performance;
        } else {
            knearest->setK(i);
            performance = knearest->validatePerformance();
            if(performance > bestperformance) {
                bestperformance = performance;
                bestk = i;
            }
        }
    }
    knearest->setK(bestk);
    knearest->testPerformance();
}

void test3() {
    dataHandler *dh = new dataHandler();
    dh->readFeatureVector("../../DataSet/train-images.idx3-ubyte");
    dh->readFeatureLabels("../../DataSet/train-labels.idx1-ubyte");
    dh->splitData();
    dh->countClasses();
    double performance = 0.0;
    double bestperformance = 0.0;
    int bestk = 1;
    for(int k = dh->getClassCounts(); k < dh->getTrainingData()->size() * 0.1; k++) {
        kmeans *km = new kmeans(k);
        km->setTrainingData(dh->getTrainingData());
        km->setTestData(dh->getTestData());
        km->setValidationData(dh->getValidationData());
        km->initClusters();
        km->train();
        performance = km->validate();
        printf("Current Performance @ K = %d: %.2f\n", k, performance);
        if(performance > bestperformance) {
            bestperformance = performance;
            bestk = k;
        }
    }
        kmeans *km = new kmeans(bestk);
        km->setTrainingData(dh->getTrainingData());
        km->setTestData(dh->getTestData());
        km->setValidationData(dh->getValidationData());
        km->initClusters();
        km->test();
        performance = km->test();
        printf("Current Performance @ K = %d: %.2f\n", bestk, performance);
}

void test4() {
    dataHandler *dh = new dataHandler();
    dh->readCSV("../../DataSet/iris.data", ",");

    dh->splitData();
    std::vector<int> hiddenLayers = {10};
    auto lambda = [&] () {
        network *net = new network(
            hiddenLayers, 
            dh->getTrainingData()->at(0)->getNormalizedFeatureVector()->size(), 
            dh->getClassCounts(),
            0.25);
        net->setTrainingData(dh->getTrainingData());
        net->setTestData(dh->getTestData());
        net->setValidationData(dh->getValidationData());
        net->train(15);
        net->validate();
        printf("Test Performance: %.3f\n", net->test());
    };
    lambda();
}

int main(int, char**){
    // test1();
    // test2();
    // test3();
    test4();
}
