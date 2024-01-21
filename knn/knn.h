#ifndef _KNN_H_
#define _KNN_H_

#include <vector>
#include "../DataHandle/inputdata.h"

class knn : public common {
private:
    int k;
    std::vector<data *> * neighbours;

public:
    knn(int val);
    knn();
    ~knn();

    void findKNearest(data *querypoint);
    void setK(int val);

    int predict();
    double calculateDistance(data *querypoint, data *input);
    double validatePerformance();
    double testPerformance();
};

#endif