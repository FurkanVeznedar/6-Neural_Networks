#include "layer.h"

layer::layer(int previouslayersize, int currentlayersize) {
    for(int i = 0; i < currentlayersize; i++) {
        neurons.push_back(new neuron(previouslayersize));
    }
    this->currentlayersize = currentlayersize;
}