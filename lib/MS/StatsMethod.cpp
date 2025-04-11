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

        void StatsMethod::makeNoise(int numNoise, Distrib& dist){
            auto targetSize = _targetMatrix.size();
            assert(targetSize && "Target matrix is empty");
            assert((numNoise >= 0 && numNoise <= targetSize) && "numNoise must be somewhere in between 0 and target size");

            // this one for noise;
            std::random_device rd;
            std::mt19937 gen(rd());

            //this one for index in _targetMatrix;
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> randomIdx(0, targetSize * 100);
            std::vector<bool> noised(targetSize, false);

            int countNoised = 0;
            // may take a while if we want to noise ~50%;
            while (countNoised < numNoise) {
                auto currentIndex = randomIdx(dev);
                while (noised[currentIndex%targetSize]) {
                    currentIndex = randomIdx(dev);
                }
                std::cout << "attempt " << countNoised << " index " << currentIndex << std::endl; 
                noised[currentIndex%targetSize] = true;
                ++countNoised;
                _targetMatrix[currentIndex%targetSize] += dist.generate(gen);
            }
        }

    } // namespace MS;
} // namespace CP;