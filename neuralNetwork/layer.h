#ifndef _LAYER_H_
#define _LAYER_H_

#include "neuron.h"
#include <stdint.h>
#include <vector>

static int layerId = 0;

class layer {
public:
    int currentlayersize;
    std::vector<neuron *> neurons;
    std::vector<double> layeroutput;

    layer(int, int);
    ~layer();
};

#endif // _LAYER_H_