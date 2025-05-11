#ifndef CP_2025_STATSMETHOD_H
#define CP_2025_STATSMETHOD_H

#include "../COMMON/FileParser.hpp"
#include "../COMMON/Matrix.hpp"
#include "../DISTRIBUTIONS/ErrorDistributions.hpp"
#include "Eigen/Dense"

#include <cassert>
#include <random>

namespace CP {
    namespace MS {
        using RData = Common::RegressionData;
        using Distrib = Distributions::ErrorDistributions;
        class StatsMethod {
        public:
            explicit StatsMethod(const RData& data, const std::string& n, double delta=1, double ep=1000, double lr=0.001);
            virtual Eigen::VectorXd compute() { return Eigen::VectorXd::Zero(_featuresMatrix.cols()); }

        protected:
            Eigen::MatrixXd _featuresMatrix;
            Eigen::VectorXd _targetMatrix;
            double _delta;
            double _epochs;
            double _learningRate;
            std::string _name;
        };
    } // namespace MS;
} // namespace CP;


#endif //CP_2025_STATSMETHOD_H