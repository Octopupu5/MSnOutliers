#include "StatsMethod.hpp"

CP::StatsMethod::StatsMethod(const RData &data) {
    if (data.empty()) {
        std::cout << "Error, data is empty" << std::endl;
        return;
    }
    uint32_t numFeatures = data[0].features.size() + 1;
    uint32_t numSamples = data.size();
    featuresMatrix_ = Matrix(numSamples, numFeatures);
    targetMatrix_ = Matrix(numSamples, 1);
    for (uint32_t i = 0; i < numSamples; ++i) {
        for (uint32_t j = 0; j < numFeatures; ++j) {
            if (!j) {
                featuresMatrix_.At(i, j) = 1;
            } else {
                featuresMatrix_.At(i, j) = data[i].features[j-1];
            }
        }
        targetMatrix_.At(i, 0) = data[i].target;
    }
}