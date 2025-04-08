#include "StatsMethod.hpp"

namespace CP {
    namespace MS {
        StatsMethod::StatsMethod(const RData &data) {
            if (data.empty()) {
                std::cout << "Error, data is empty" << std::endl;
                return;
            }
            auto numFeatures = static_cast<Eigen::Index>(data[0].features.size() + 1);
            auto numSamples = static_cast<Eigen::Index>(data.size());
            _featuresMatrix.resize(numSamples, numFeatures);
            _targetMatrix.resize(numSamples);
            for (Eigen::Index i = 0; i < numSamples; ++i) {
                for (Eigen::Index j = 0; j < numFeatures; ++j) {
                    if (!j) {
                        _featuresMatrix(i, j) = 1;
                    } else {
                        _featuresMatrix(i, j) = data[i].features[j-1];
                    }
                }
                _targetMatrix[i] = data[i].target;
            }
        }
    } // namespace MS;
} // namespace CP;