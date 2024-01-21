#include "kmeans.h"

kmeans::kmeans(int k) {
    numclusters = k;
    clusters = new std::vector<cluster_t *>;
    usedindexes = new std::unordered_set<int>;
}

void kmeans::initClusters() {
    for(int i = 0; i < numclusters; i++) {
        int index = (rd() % trainingdata->size());
        while(usedindexes->find(index) != usedindexes->end()) {
            index = (rd() % trainingdata->size());
        }
        clusters->push_back(new cluster(trainingdata->at(index)));
        usedindexes->insert(index);
    }
}

void kmeans::initClustersForEachClass() {
    std::unordered_set<int> classesused;
    for(int i = 0; i < trainingdata->size(); i++) {
        if(classesused.find(trainingdata->at(i)->getLabel()) == classesused.end()) {
            clusters->push_back(new cluster_t(trainingdata->at(i)));
            classesused.insert(trainingdata->at(i)->getLabel());
            usedindexes->insert(i);
        }
    }
}

void kmeans::train() {
    int index = 0;
    while (usedindexes->size() < trainingdata->size()) {   
        int index = (rd() % trainingdata->size());
        while(usedindexes->find(index) != usedindexes->end()) {
            index = (rd() % trainingdata->size());
        }
        double mindist = std::numeric_limits<double>::max();
        int bestcluster = 0;
        for(int j = 0; j < clusters->size(); j++) {
            double currentdist = euclideanDistance(clusters->at(j)->centroid, trainingdata->at(index)); // out of range
            if(currentdist < mindist) {
                mindist = currentdist;
                bestcluster = j;
            }
        }
        clusters->at(bestcluster)->addToCluster(trainingdata->at(index));
        usedindexes->insert(index);
    }
    
}

double kmeans::euclideanDistance(std::vector<double> *centroid, data *point) {
    double dist = 0.0;
    for(int i = 0; i < centroid->size(); i++) {
        dist += pow(centroid->at(i) - point->getFeatureVector()->at(i), 2);
    }
    return sqrt(dist);
}

double kmeans::validate() {
    double numcorrect = 0.0;
    for(auto querypoint : *validationdata) {
        double mindist = std::numeric_limits<double>::max();
        int bestcluster = 0;
        for(int j = 0; j < clusters->size(); j++) {
            double currentdist = euclideanDistance(clusters->at(j)->centroid, querypoint);
            if(currentdist < mindist) {
                mindist = currentdist;
                bestcluster = j;
            }
        }
        if(clusters->at(bestcluster)->mostfrequentclass == querypoint->getLabel()) numcorrect++;
    }
    return 100.0 * (numcorrect / (double)validationdata->size());
}

double kmeans::test() {
    double numcorrect = 0.0;
    for(auto querypoint : *testdata) {
        double mindist = std::numeric_limits<double>::max();
        int bestcluster = 0;
        for(int j = 0; j < clusters->size(); j++) {
            double currentdist = euclideanDistance(clusters->at(j)->centroid, querypoint);
            if(currentdist < mindist) {
                mindist = currentdist;
                bestcluster = j;
            }
        }
        if(clusters->at(bestcluster)->mostfrequentclass == querypoint->getLabel()) numcorrect++;
    }
    return 100.0 * (numcorrect / (double)testdata->size());
}
