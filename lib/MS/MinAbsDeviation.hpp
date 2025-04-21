#ifndef CP_2025_MIN_ABS_DEV_H
#define CP_2025_MIN_ABS_DEV_H

#include "StatsMethod.hpp"

namespace CP {
    namespace MS {
        using Vec = Eigen::VectorXd;
        using Mat = Eigen::MatrixXd;
        class MinAbsDeviation : public StatsMethod {
        public:
            explicit MinAbsDeviation(const RData& data, double eps, double lr): StatsMethod(data, "LAD", 1, eps, lr) {}
            Vec compute() override;
        };
    } // namespace MS;
} // namespace CP;


#endif