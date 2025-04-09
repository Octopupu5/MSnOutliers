#ifndef CP_2025_STATSMETHOD_H
#define CP_2025_STATSMETHOD_H

#include "../COMMON/FileParser.hpp"
#include "../COMMON/Matrix.hpp"
#include "Eigen/Dense"

namespace CP {
    namespace MS {
        using RData = Common::RegressionData;
        class StatsMethod {
        public:
            explicit StatsMethod(const RData& data);
            virtual Eigen::VectorXd compute() = 0;

        protected:
            Eigen::MatrixXd _featuresMatrix;
            Eigen::VectorXd _targetMatrix;
        };
    } // namespace MS;
} // namespace CP;


#endif //CP_2025_STATSMETHOD_H