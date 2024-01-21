#ifndef _COMMON_H_
#define _COMMON_H_

#include <vector>
#include "data.h"

class common {
protected:
    std::vector<data *> * trainingdata;
    std::vector<data *> * testdata;
    std::vector<data *> * validationdata;

public:
    void setTrainingData(std::vector<data *> *vect);
    void setTestData(std::vector<data *> *vect);
    void setValidationData(std::vector<data *> *vect);

};

#endif