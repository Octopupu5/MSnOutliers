#include "DataDeNoiser.hpp"

namespace CP {
    namespace Common {
        RData DataDeNoiser::deNoise(int numNoise, CP::Distributions::ErrorDistributions& dist, const std::string& mlModelType) {
            auto targetSize = static_cast<int>(_originalData.Rows());
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
                _originalData.At(i, _originalData.Cols() - 1) += noise;
            }

            if (mlModelType == "IForest") {
                CP::ML::iForest forest(350, 15);
                forest.Fit(_originalData);
                for (size_t i = 0; i < _originalData.Rows(); ++i) {
                    CP::Common::Matrix sample = _originalData.RowMatrix(i);
                    double proba = forest.PredictProba(sample);
                }
                // CP::Common
            } else if (mlModelType == "DBSCAN") {
                CP::ML::DBSCAN dbscan(40, 5); 
                dbscan.Fit(_originalData);
                std::vector<int32_t> idToCluster = dbscan.getIdToCluster();

            } else if (mlModelType == "OCSVM") {
                CP::ML::OneClassSVM svm(1.0 / _originalData.Cols());
                svm.Fit(_originalData);
                CP::Common::Matrix answers = svm.Predict(_originalData);
                RData cleanedData;
                for (size_t i = 0; i < answers.Rows(); ++i) {
                    if (!answers.At(i, 0).Value()) {
                        cleanedData.push_back(_originalRData[i]);
                    }
                }
                return cleanedData;
            } else {
                return _originalRData;
            }
        }
    }
}