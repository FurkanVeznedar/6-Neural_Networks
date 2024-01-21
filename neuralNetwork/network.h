#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "..\DataHandle\common.h"
#include "neuron.h"
#include "layer.h"
#include <cmath>
#include <vector>

class network : public common {
public:
    std::vector<layer *> layers;
    double learningrate;
    double testperformance;

    network(std::vector<int> spec, int, int, double);
    ~network();

    std::vector<double> fProp(data *data);
    double activate(std::vector<double>, std::vector<double>);  // dot product
    double transfer(double);
    double transferDerivative(double);  // used for backprop
    int predict(data *data); // return the index of the maximum value in the output array

    void bProp(data *data);
    void updateWeights(data *data);

    void train(int);  // num iterations
    double test();
    void validate();

};

#endif _NETWORK_H_