#include "network.h"

network::network(std::vector<int> spec, int inputsize, int numclasses, double learningrate) {
    for(int i = 0; i < spec.size(); i++) {
        if(i == 0) {
            layers.push_back(new layer(inputsize, spec.at(i)));
        } else {
            layers.push_back(new layer(layers.at(i - 1)->neurons.size(), spec.at(i)));
        }
    }
    layers.push_back(new layer(layers.at(layers.size() - 1)->neurons.size(), numclasses));
    this->learningrate = learningrate;
}

network::~network() {

}

double network::activate(std::vector<double> weights, std::vector<double> input) {
    double activation = weights.back();   // bias term
    for(int i = 0; i < weights.size() - 1; i++) {
        activation += weights[i] * input[i];
    }
    return activation;
}

double network::transfer(double activation) {
    return 1.0 / (1.0 + exp(-activation));
}

double network::transferDerivative(double output) {
    return output * (1 - output);
}

std::vector<double> network::fProp(data *data) {
    std::vector<double> inputs = *data->getNormalizedFeatureVector();
    for(int i = 0; i < layers.size(); i++) {
        layer *layer = layers.at(i);
        std::vector<double> newinputs;
        for(neuron *n : layer->neurons) {
            double activation = this->activate(n->weights, inputs);
            n->output = this->transfer(activation);
            newinputs.push_back(n->output);
        }
        inputs = newinputs;
    }
    return inputs;  // output layer outputs
}

void network::bProp(data *data) {
    for(int i = layers.size() - 1; i >= 0; i--) {
        layer *layer = layers.at(i);
        std::vector<double> errors;
        if(i != layers.size() - 1) {
            for(int j = 0; j < layer->neurons.size(); j++) {
                double error = 0.0;
                for(neuron *n : layers.at(i + 1)->neurons) {
                    error += (n->weights.at(j) * n->delta);
                }
                errors.push_back(error);
            }
        } else {
            for(int j = 0; j < layer->neurons.size(); j++) {
                neuron *n = layer->neurons.at(j);
                errors.push_back((double)data->getClassVector().at(j) - n->output); // expected - actual
            }
        }
        for(int j = 0; j < layer->neurons.size(); j++) {
            neuron *n = layer->neurons.at(j);
            n->delta = errors.at(j) * this->transferDerivative(n->output); // gradient / derivative part pf back prop
        }
    }
}

void network::updateWeights(data *data) {
    std::vector<double> inputs = *data->getNormalizedFeatureVector();
    for(int i = 0; i < layers.size(); i++) {
        if(i != 0) {
            for(neuron *n : layers.at(i - 1)->neurons) {
                inputs.push_back(n->output);
            }
        }
        for(neuron *n : layers.at(i)->neurons) {
            for(int j = 0; j < inputs.size(); j++) {
                n->weights.at(j) += this->learningrate * n->delta * inputs.at(j);
            }
            n->weights.back() += this->learningrate * n->delta;
        }
        inputs.clear();
    }

}

int network::predict(data *data) {
    std::vector<double> outputs = fProp(data);
    return std::distance(outputs.begin(), std::max_element(outputs.begin(), outputs.end()));
}

void network::train(int numepochs) {
    for(int i = 0; i < numepochs; i++) {
        double sumerror = 0.0;
        for(data *data : *this->trainingdata) {
            std::vector<double> outputs = fProp(data);
            std::vector<int> expected = data->getClassVector();
            double temperrorsum = 0.0;
            for(int j = 0; j < outputs.size(); j++) {
                temperrorsum += pow(((double)expected.at(j) - outputs.at(j)), 2);
            }
            sumerror += temperrorsum;
            bProp(data);
            updateWeights(data);
        }
        printf("Iteration: %d \t Error=%.4f\n", i, sumerror);
    }
}

double network::test() {
    double numcorrect = 0.0;
    double count = 0.0;
    for(data *data : *this->testdata) {
        count++;
        int index = predict(data);
        if(data->getClassVector().at(index) == 1) numcorrect++;
    }

    testperformance = (numcorrect / count);
    return testperformance;
}

void network::validate() {
    double numcorrect = 0.0;
    double count = 0.0;
    for(data *data : *this->validationdata) {
        count++;
        int index = predict(data);
        if(data->getClassVector().at(index) == 1) numcorrect++;
    }
    printf("Validation Performance: %.4f\n", numcorrect / count);
}