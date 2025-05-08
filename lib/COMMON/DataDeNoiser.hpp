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
            DataDeNoiser(const RData& data);
            void noise(int numNoise, CP::Distributions::ErrorDistributions& dist);
            RData denoise(const std::string& mlModelType);
        private:
            void iForestDenoiser(RData& cleanedData);
            void KDEDenoiser(RData& cleanedData);
            void KNNDenoiser(RData& cleanedData);
            void DBSCANDenoiser(RData& cleanedData);
            const RData& _data;
            RData _dataNoised;
            Matrix _dataMatNoised;
        };
    }
}

#endif // DATADENOISER_HPP