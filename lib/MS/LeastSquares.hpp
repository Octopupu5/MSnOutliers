#ifndef CP_BACK_LEASTSQUARES_H
#define CP_BACK_LEASTSQUARES_H

#include "../COMMON/FileParser.hpp"
#include "StatsMethod.hpp"

namespace CP {
    namespace MS {
        using RData = Common::RegressionData;
        using Vec = Eigen::VectorXd;

        class LeastSquaresMethod : public StatsMethod {
        public:
            explicit LeastSquaresMethod(const RData& data): StatsMethod(data, "LSM"){}
            Vec compute() override;
        };
    } // namespace MS;
} // namespace CP;

#endif