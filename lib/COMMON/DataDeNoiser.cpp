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
                double k = 0.1;
                double eps = k * std::sqrt(cols) * std::sqrt(std::accumulate(_originalData.begin(), _originalData.end(), 0.0, [](double sum, const std::vector<Feature>& row) {
                    double row_sum = 0;
                    for (const auto& val : row) {
                        row_sum += val.Value() * val.Value();
                    }
                    return sum + row_sum;
                }) / rows);
                uint32_t min_samples = static_cast<uint32_t>(std::log(rows));
                CP::ML::DBSCAN dbscan(eps, min_samples); 
                dbscan.Fit(_originalData);
                std::vector<int32_t> idToCluster = dbscan.getIdToCluster();
                auto [rows, cols] = Shape(_originalData);
                for (size_t i = 0; i < rows; ++i) {
                    if (idToCluster[i] != 0) {
                        cleanedData.push_back(_originalRData[i]);
                    }
                }
            } else if (mlModelType == "KDE") {
                CP::ML::KernelDensityEstimator kde(1.0 / cols);
                kde.Fit(_originalData);
                CP::Common::Matrix answers = kde.Predict(_originalData);
                for (size_t i = 0; i < rows; ++i) {
                    if (!answers[i][0].Value()) {
                        cleanedData.push_back(_originalRData[i]);
                    }
                }
            } else if (mlModelType == "KNN") {
                CP::ML::KNN knn(10, 0.15);
                knn.Fit(_originalData);
                std::vector<std::pair<double, size_t>> preds = knn.PairDistances();
                double thres = knn.Threshold();
                for (size_t i = 0; i < rows; ++i) {
                    if (preds[i].first < thres) {
                        cleanedData.push_back(_originalRData[preds[i].second]);
                    }
                }
            } else if (mlModelType == "None") {
                return _originalRData;
            }
            return cleanedData;
        }
    }
}