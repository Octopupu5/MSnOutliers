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
            explicit StatsMethod(const RData& data, const std::string& n);
            void makeNoise(int percent, Distrib& dist);
            virtual Eigen::VectorXd compute() { return Eigen::VectorXd::Zero(_featuresMatrix.cols()); }

        protected:
            Eigen::MatrixXd _featuresMatrix;
            Eigen::VectorXd _targetMatrix;
            std::string _name;
        };
    } // namespace MS;
} // namespace CP;


#endif //CP_2025_STATSMETHOD_H