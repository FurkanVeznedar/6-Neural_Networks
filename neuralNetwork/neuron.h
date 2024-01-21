#ifndef _NEURON_H_
#define _NEURON_H_

#include <stdio.h>
#include <cmath>
#include <vector>
#include <random>

class neuron {
public:
    double output;
    double delta;
    std::vector<double> weights;

    neuron(int);

    void initializeWeights(int);
};

#endif _NEURON_H_