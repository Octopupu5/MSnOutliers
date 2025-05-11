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
            RData denoise(const std::string& mlModelType, double param1, double param2);
            double Precision();
            double Recall();
            double F1();
            std::vector<bool> _noisedIndices;
            std::vector<bool> _denoisedIndices;
        private:
            void IForestDenoiser(RData& cleanedData, int nEstimators, int depth);
            void KDEDenoiser(RData& cleanedData, double gamma);
            void KNNDenoiser(RData& cleanedData, int k, double contamination);
            void DBSCANDenoiser(RData& cleanedData, double r, int minClusterSize);
            void CalculateMetrics();
            const RData& _data;
            RData _dataNoised;
            Matrix _dataMatNoised;
            double _precision;
            double _recall;
            double _f1Score;
        };
    }
}

#endif // DATADENOISER_HPP