#ifndef DATADENOISER_HPP
#define DATADENOISER_HPP

#include "../DISTRIBUTIONS/ErrorDistributions.hpp"
#include "../ML/DBSCAN/DBSCAN.hpp"
#include "../ML/KDE/KDE.hpp"
#include "../ML/iForest/iForest.hpp"
#include "../ML/KNN/Knn.hpp"
#include "FileParser.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
#include <cassert>

namespace CP {
    namespace Common {
        using RData = std::vector<RegressionUnit>;
        class DataDeNoiser {
        public:
            DataDeNoiser(const RData& data) : _originalRData(data) {
                if (!data.size()) {
                    throw std::runtime_error("No data!");
                }
                size_t rows = data.size();
                size_t cols = data[0].features.size();
                _originalData = Matrix(rows, Row(cols + 1));
                for (size_t i = 0; i < rows; ++i) {
                    for (size_t j = 0; j < cols; ++j) {
                        _originalData[i][j] = data[i].features[j];
                    }
                    _originalData[i][cols] = data[i].target;
                }
            }
            RData deNoise(int numNoise, CP::Distributions::ErrorDistributions& dist, const std::string& mlModelType);
        private:
            Matrix _originalData;
            RData _originalRData;
        };
    }
}

#endif // DATADENOISER_HPP