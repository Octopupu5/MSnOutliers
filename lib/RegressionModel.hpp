#ifndef REGRESSION_MODEL_H
#define REGRESSION_MODEL_H

#include <string>

class RegressionModel {
public:
    void loadData(const std::string& filepath);
    void train();
    void predict();
};

#endif // REGRESSION_MODEL_H