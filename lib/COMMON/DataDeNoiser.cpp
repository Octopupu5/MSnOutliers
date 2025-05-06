#include "DataDeNoiser.hpp"

namespace CP {
    namespace Common {
        RData DataDeNoiser::deNoise(int numNoise, CP::Distributions::ErrorDistributions& dist, const std::string& mlModelType) {
            auto [rows, cols] = Shape(_originalData);
            auto targetSize = static_cast<int>(rows);
            assert(targetSize && "Target data is empty");
            assert((numNoise >= 0 && numNoise <= targetSize) && "numNoise must be somewhere in between 0 and target size");
        
            std::random_device rd;
            std::mt19937 gen(rd());

            std::vector<size_t> indices(targetSize);
            for (int i = 0; i < targetSize; ++i) {
                indices[i] = i;
            }

            std::shuffle(indices.begin(), indices.end(), gen);
            
            for (int i = 0; i < numNoise; ++i) {
                size_t currentIndex = indices[i];
                double noise = dist.generate(gen);
                _originalRData[currentIndex].target += noise;
                _originalData[i][cols - 1] += noise;
            }
            
            RData cleanedData;
            if (mlModelType == "IForest") {
                CP::ML::iForest forest(100, 12);
                forest.Fit(_originalData);
                std::vector<std::pair<double, size_t>> probs;
                for (size_t i = 0; i < rows; ++i) {
                    probs.push_back({forest.PredictProba(_originalData[i]), i});
                }

                size_t sizeToClean = (size_t) (0.15 * _originalData.size());
                std::nth_element(probs.begin(), probs.begin() + sizeToClean, probs.end());
                for (size_t i = 0; i < rows; ++i) {
                    if (probs[i].first > probs[sizeToClean].first) {
                        cleanedData.push_back(_originalRData[probs[i].second]);
                    }
                }
            } else if (mlModelType == "DBSCAN") {
                CP::ML::DBSCAN dbscan(40, 5); 
                dbscan.Fit(_originalData);
                std::vector<int32_t> idToCluster = dbscan.getIdToCluster();

            } else if (mlModelType == "OCSVM") {
                CP::ML::OneClassSVM svm(1.0 / cols);
                svm.Fit(_originalData);
                CP::Common::Matrix answers = svm.Predict(_originalData);
                for (size_t i = 0; i < rows; ++i) {
                    if (!answers[i][0].Value()) {
                        cleanedData.push_back(_originalRData[i]);
                    }
                }
            } else if (mlModelType == "None") {
                return _originalRData;
            }
            return cleanedData;
        }
    }
}