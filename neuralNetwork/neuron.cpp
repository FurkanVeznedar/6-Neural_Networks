#include "neuron.h"

double generateRandomNumber(double min, double max) {
    double random = (double) rand() / RAND_MAX;
    return min + random * (max - min);
}

neuron::neuron(int previouslayersize) {
    initializeWeights(previouslayersize);
}

void neuron::initializeWeights(int previouslayersize) {
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    for(int i = 0; i < previouslayersize + 1; i++) {
        weights.push_back(generateRandomNumber(-1.0, 1.0));
    }
}

