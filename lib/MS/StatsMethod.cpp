#include "StatsMethod.hpp"

namespace CP {
    namespace MS {
        StatsMethod::StatsMethod(const RData &data) {
            if (data.empty()) {
                std::cout << "Error, data is empty" << std::endl;
                return;
            }
            uint32_t numFeatures = data[0].features.size() + 1;
            uint32_t numSamples = data.size();
            _featuresMatrix = Common::Matrix(numSamples, numFeatures);
            _targetMatrix = Common::Matrix(numSamples, 1);
            
            for (uint32_t i = 0; i < numSamples; ++i) {
                for (uint32_t j = 0; j < numFeatures; ++j) {
                    if (!j) {
                        _featuresMatrix.At(i, j).SetValue(1);
                    } else {
                        _featuresMatrix.At(i, j).SetValue(data[i].features[j-1]);
                    }
                }
                _targetMatrix.At(i, 0) = data[i].target;
            }
        }
    }
}