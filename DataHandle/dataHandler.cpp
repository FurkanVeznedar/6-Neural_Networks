#include "dataHandler.h"
#include "data.h"

dataHandler::dataHandler() {
    dataarray = new std::vector<data *>;
    testdata = new std::vector<data *>;
    trainingdata = new std::vector<data *>;
    validationdata = new std::vector<data *>;
}
dataHandler::~dataHandler() {
    // FREE dynamically Allocated STUFF
    delete dataarray;
    delete testdata;
    delete trainingdata;
    delete validationdata;
}

void dataHandler::readCSV(std::string path, std::string delimiter) {
    numclasses = 0;
    std::ifstream datafile;
    datafile.open(path.c_str());
    std::string line; // holds each line

    while(std::getline(datafile, line)) {
        if(line.length() == 0) continue;
        data *d = new data();
        d->setNormalizedFeatureVector(new std::vector<double>());
        size_t pos = 0;
        std::string token; // value in between delimeter
        while((pos = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, pos);
            d->appendToFeatureVector(std::stod(token));
            line.erase(0, pos + delimiter.length());
        }
        if(classfromstring.find(line) != classfromstring.end()) {
            d->setLabel(classfromstring[line]);
        } else {
            classfromstring[line] = numclasses;
            d->setLabel(classfromstring[token]);
            numclasses++;
        }
        dataarray->push_back(d);
    }
    for(data *data : *dataarray) data->setClassVector(numclasses);

    featurevectorsize = dataarray->front()->getNormalizedFeatureVector()->size();
}

void dataHandler::readFeatureVector(std::string path) {
    uint32_t header[4];  // |MAGIC|NUM IMAGES|ROWSIZE|COLSIZE
    unsigned char bytes[4];
    FILE *f = fopen(path.c_str(), "rb");

    if(f) {
        for(int i = 0; i < 4; i++) {
            if(fread(bytes, sizeof(bytes), 1, f)) {
                header[i] = converttolittleendian(bytes);
            }
        }
        printf("Done getting Input File Header.\n");
        int imagesize = header[2] * header[3];
        for(int i = 0; i < header[1]; i++) {
            data *d = new data();
            uint8_t element[1];
            for(int j = 0; j < imagesize; j++) {
                if(fread(element, sizeof(element), 1, f)) {
                    d->appendToFeatureVector(element[0]);
                } else {
                    printf("Error Reading from File.\n");
                    exit(1);
                }
            }
            dataarray->push_back(d);
        }
        printf("Succesfully read and stored %lu feature vectors.\n", dataarray->size());
        fclose(f);
    } else {
        printf("Could not find file.\n");
        exit(1);
    }
}

void dataHandler::readFeatureLabels(std::string path) {
    uint32_t header[2];  // |MAGIC|NUM IMAGES
    unsigned char bytes[4];
    FILE *f = fopen(path.c_str(), "rb");

    if(f) {
        for(int i = 0; i < 2; i++) {
            if(fread(bytes, sizeof(bytes), 1, f)) {
                header[i] = converttolittleendian(bytes);
            }
        }
        printf("Done Getting Label File Header.\n");
        for(int i = 0; i < header[1]; i++) {
            uint8_t element[1];
            if(fread(element, sizeof(element), 1, f)) {
                dataarray->at(i)->setLabel(element[0]);
            } else {
                printf("Error Reading from File.\n");
                exit(1);
            }
        }
        printf("Succesfully read and stored labels.\n");
        fclose(f);
    } else {
        printf("Could not find file.\n");
        exit(1);
    }
}

void dataHandler::splitData() {
    std::unordered_set<int> usedindexes;
    int trainsize = dataarray->size() * TRAIN_SET_PERCENT;
    int testsize = dataarray->size() * TEST_SET_PERCENT;
    int validsize = dataarray->size() * VALIDATION_PERCENT;

    // Training Data

    int count = 0;
    while (count < trainsize) {
        int randindex = rd() % dataarray->size(); // 0 & dataarray->size() - 1
        if (usedindexes.find(randindex) == usedindexes.end()) {
            trainingdata->push_back(dataarray->at(randindex));
            usedindexes.insert(randindex);
            count++;
        }
        //printf("...%d\n", count);
    }

    // Test Data

    count = 0;
    while (count < testsize) {
        int randindex = rd() % dataarray->size(); // 0 & dataarray->size() - 1
        if (usedindexes.find(randindex) == usedindexes.end()) {
            testdata->push_back(dataarray->at(randindex));
            usedindexes.insert(randindex);
            count++;
        }
    }

    // Validation Data

    // Test Data

    count = 0;
    while (count < validsize) {
        int randindex = rd() % dataarray->size(); // 0 & dataarray->size() - 1
        if (usedindexes.find(randindex) == usedindexes.end()) {
            validationdata->push_back(dataarray->at(randindex));
            usedindexes.insert(randindex);
            count++;
        }
    }
    
    printf("Training Data Size: %lu.\n", trainingdata->size());
    printf("Test Data Size: %lu.\n", testdata->size());
    printf("Validation Data Size: %lu.\n", validationdata->size());
}

void dataHandler::countClasses() {
    int count = 0;
    for(unsigned i = 0; i < dataarray->size(); i++) {
        if(classfromint.find(dataarray->at(i)->getLabel()) == classfromint.end()) {
            classfromint[dataarray->at(i)->getLabel()] = count;
            dataarray->at(i)->setEnumeratedLabel(count);
            count++;
        }
    }
    numclasses = count;
    for(data *data: *dataarray) data->setClassVector(numclasses);
    printf("Succesfully Extracted %d Unique Classes.\n", numclasses);
}

void dataHandler::normalize() {
    std::vector<double> mins, maxs;
    //Fill min and max lists

    data *d = dataarray->at(0);
    for(auto val : *d->getFeatureVector()) {
        mins.push_back(val);
        maxs.push_back(val);
    }
    
    for(int i = 1; i < dataarray->size(); i++) {
        d = dataarray->at(i);
        for(int j = 0; j < d->getFeatureVectorSize(); j++) {
            double value = (double)d->getFeatureVector()->at(j);
            if(value < mins.at(j)) mins[j] = value;
            if(value < maxs.at(j)) mins[j] = value;
        }
    }
    // normalize data array

    for(int i = 0; i < dataarray->size(); i++) {
        dataarray->at(i)->setNormalizedFeatureVector(new std::vector<double>());
        dataarray->at(i)->setClassVector(numclasses);
        for(int j = 0; j < dataarray->at(i)->getFeatureVectorSize(); j++) {
            if(maxs[j] - mins[j] == 0) dataarray->at(i)->appendToFeatureVector(0.0);
            else dataarray->at(i)->appendToFeatureVector((double)(dataarray->at(i)->getFeatureVector()->at(j) - mins[j]) / (maxs[j] - mins[j]));
        }
    }
}

void dataHandler::print() {
  printf("Training Data:\n");
  for(auto data : *trainingdata) {
    for(auto value : *data->getNormalizedFeatureVector()) {
      printf("%.3f,", value);
    }
    printf(" ->   %d\n", data->getLabel());
  }
  return;

  printf("Test Data:\n");
  for(auto data : *testdata) {
    for(auto value : *data->getNormalizedFeatureVector()) {
      printf("%.3f,", value);
    }
    printf(" ->   %d\n", data->getLabel());
  }

  printf("Validation Data:\n");
  for(auto data : *validationdata) {
    for(auto value : *data->getNormalizedFeatureVector()) {
      printf("%.3f,", value);
    }
    printf(" ->   %d\n", data->getLabel());
  }
}

uint32_t dataHandler::converttolittleendian(const unsigned char* bytes) {
    return (uint32_t) ((bytes[0] << 24) | 
                       (bytes[1] << 16) |
                       (bytes[2] << 8) |
                       (bytes[3]));
}

int dataHandler::getClassCounts() {
    return numclasses;
}

int dataHandler::getDataArraySize() {
    return dataarray->size();
}

int dataHandler::getTrainingDataSize() {
    return trainingdata->size();
}

int dataHandler::getTestDataSize() {
    return testdata->size();
}   

int dataHandler::getValidationSize() {
    return validationdata->size();
}

std::vector<data *> * dataHandler::getTrainingData() {
    return trainingdata;
}

std::vector<data *> * dataHandler::getTestData() {
    return testdata;
}

std::vector<data *> * dataHandler::getValidationData() {
    return validationdata;
}

std::map<uint8_t, int> dataHandler::getClassMap() {
    return classfromint;
}