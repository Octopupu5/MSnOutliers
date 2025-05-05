#ifndef DATADENOISER_HPP
#define DATADENOISER_HPP

#include "../DISTRIBUTIONS/ErrorDistributions.hpp"
#include "../ML/DBSCAN/DBSCAN.hpp"
#include "../ML/OCSVM/OneClassSVM.hpp"
#include "../ML/iForest/iForest.hpp"
#include "Matrix.hpp"
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
                _originalData = Matrix(data.size(), data.size() ? data[0].features.size() + 1 : 0);
                for (size_t i = 0; i < _originalData.Rows(); ++i) {
                    for (size_t j = 0; j < _originalData.Cols() - 1; ++j) {
                        _originalData.At(i, j).SetValue(data[i].features[j]);
                    }
                    _originalData.At(i, _originalData.Cols() - 1).SetValue(data[i].target);
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