#ifndef _KMEANS_H_
#define _KMEANS_H_

#include <unordered_set>
#include <limits>
#include <cstdlib>
#include <cmath>
#include <map>
#include <random>
#include "../DataHandle/inputdata.h"

typedef struct cluster {
    std::vector<double> *centroid;
    std::vector<data *> *clusterpoints;
    std::map<int, int> classcounts;
    int mostfrequentclass;

    cluster(data *initialpoint) {
        centroid = new std::vector<double>;
        clusterpoints = new std::vector<data *>;
        for(auto value : *(initialpoint->getFeatureVector())) {
            centroid->push_back(value);
        }
        clusterpoints->push_back(initialpoint);
        classcounts[initialpoint->getLabel()] = 1;
        mostfrequentclass = initialpoint->getLabel();
    }

    void addToCluster(data *point) {
        int previoussize = clusterpoints->size();
        clusterpoints->push_back(point);
        for(int i = 0; i < centroid->size(); i++) {
            double value = centroid->at(i);
            value *= previoussize;
            value += point->getFeatureVector()->at(i);
            value /= (double)clusterpoints->size();
            centroid->at(i) = value;
        }
        if(classcounts.find(point->getLabel()) == classcounts.end()) {
            classcounts[point->getLabel()] = 1;
        } else {
            classcounts[point->getLabel()]++;
        }
        setMostFrequentClass();
    }

    void setMostFrequentClass() {
        int bestclass = 0;
        int freq = 0;
        for(auto kv : classcounts) {
            if(kv.second > freq) {
                freq = kv.second;
                bestclass = kv.first;
            }
        }
        mostfrequentclass = bestclass;
    }

} cluster_t;

class kmeans : public common {
private:
    int numclusters;
    std::vector<cluster_t *> *clusters;
    std::unordered_set<int> *usedindexes;
    std::random_device rd;

public:
    kmeans(int k);
    void initClusters();
    void initClustersForEachClass();
    void train();
    double euclideanDistance(std::vector<double> *, data *);
    double validate();
    double test();
};

#endif // _KMEANS_H_