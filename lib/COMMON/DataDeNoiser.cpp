#include "DataDeNoiser.hpp"

namespace CP {
    namespace Common {
        DataDeNoiser::DataDeNoiser(const RData& data) : _data(data) {
            if (!data.size()) {
                throw std::runtime_error("No data!");
            }
        }

        void DataDeNoiser::noise(int numNoise, CP::Distributions::ErrorDistributions& dist) {
            size_t rows = _data.size();
            
            assert((numNoise >= 0 && numNoise <= rows) && "numNoise must be somewhere in between 0 and target size");
        
            std::random_device rd;
            std::mt19937 gen(rd());

            std::vector<size_t> indices(rows);
            for (int i = 0; i < rows; ++i) {
                indices[i] = i;
            }

            std::shuffle(indices.begin(), indices.end(), gen);
            
            size_t cols = _data[0].features.size() + 1;
            _dataMatNoised = Matrix(rows, Row(cols));
            _dataNoised = RData(_data);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols - 1; ++j) {
                    _dataMatNoised[i][j] = _data[i].features[j];
                }
                _dataMatNoised[i][cols - 1] = _data[i].target;
            }

            for (int i = 0; i < numNoise; ++i) {
                size_t currentIndex = indices[i];
                double noise = dist.generate(gen);

                _dataNoised[currentIndex].target += noise;
                _dataMatNoised[currentIndex][cols - 1] += noise;
            }
        }

        RData DataDeNoiser::denoise(const std::string& mlModelType) {
            RData cleanedData;
            if (mlModelType == "IForest") {
                iForestDenoiser(cleanedData);
            } else if (mlModelType == "DBSCAN") {
                DBSCANDenoiser(cleanedData);
            } else if (mlModelType == "KDE") {
                KDEDenoiser(cleanedData);
            } else if (mlModelType == "KNN") {
                KNNDenoiser(cleanedData);
            } else if (mlModelType == "None") {
                return _dataNoised;
            }
            return cleanedData;
        }

        void DataDeNoiser::iForestDenoiser(RData& cleanedData) {
            auto [rows, cols] = Shape(_dataMatNoised);
            CP::ML::iForest forest(100, 12, _dataMatNoised);
            forest.Fit();
            std::vector<std::pair<double, size_t>> probs;
            for (size_t i = 0; i < rows; ++i) {
                probs.push_back({forest.PredictProba(_dataMatNoised[i]), i});
            }
            size_t sizeToClean = (size_t) (0.15 * _dataMatNoised.size());
            std::nth_element(probs.begin(), probs.begin() + sizeToClean, probs.end());
            for (size_t i = 0; i < rows; ++i) {
                if (probs[i].first > probs[sizeToClean].first) {
                    cleanedData.push_back(_dataNoised[probs[i].second]);
                }
            }
        }

        void DataDeNoiser::KDEDenoiser(RData& cleanedData) {
            auto [rows, cols] = Shape(_dataMatNoised);
            CP::ML::KernelDensityEstimator kde(1.0 / cols);
            kde.Fit(_dataMatNoised);
            CP::Common::Matrix answers = kde.Predict(_dataMatNoised);
            for (size_t i = 0; i < rows; ++i) {
                if (!answers[i][0].Value()) {
                    cleanedData.push_back(_dataNoised[i]);
                }
            }
        }

        void DataDeNoiser::KNNDenoiser(RData& cleanedData) {
            auto [rows, cols] = Shape(_dataMatNoised);
            CP::ML::KNN knn(10, 0.15);
            knn.Fit(_dataMatNoised);
            std::vector<std::pair<double, size_t>> preds = knn.PairDistances();
            double thres = knn.Threshold();
            for (size_t i = 0; i < rows; ++i) {
                if (preds[i].first < thres) {
                    cleanedData.push_back(_dataNoised[preds[i].second]);
                }
            }
        }

        void DataDeNoiser::DBSCANDenoiser(RData& cleanedData) {
            auto [rows, cols] = Shape(_dataMatNoised);
            double k = 0.1;
            double eps = k * std::sqrt(cols) * std::sqrt(std::accumulate(_dataMatNoised.begin(), _dataMatNoised.end(), 0.0, [](double sum, const std::vector<Feature>& row) {
                double row_sum = 0;
                for (const auto& val : row) {
                    row_sum += val.Value() * val.Value();
                }
                return sum + row_sum;
            }) / rows);
            uint32_t min_samples = static_cast<uint32_t>(std::log(rows));
            CP::ML::DBSCAN dbscan(eps, min_samples); 
            dbscan.Fit(_dataMatNoised);
            std::vector<int32_t> idToCluster = dbscan.getIdToCluster();
            for (size_t i = 0; i < rows; ++i) {
                if (idToCluster[i] != 0) {
                    cleanedData.push_back(_dataNoised[i]);
                }
            }
        }
    }
}