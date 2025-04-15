#ifndef CP_2025_MIN_ABS_DEV_H
#define CP_2025_MIN_ABS_DEV_H

#include "StatsMethod.hpp"

namespace CP {
    namespace MS {
        using Vec = Eigen::VectorXd;
        using Mat = Eigen::MatrixXd;
        class MinAbsDeviation : public StatsMethod {
        public:
            explicit MinAbsDeviation(const RData& data, double eps, double lr): StatsMethod(data), epochs_(eps), learningRate_(lr) {}
            Vec compute() override;
        private:
            double epochs_;
            double learningRate_;
        };
    } // namespace MS;
} // namespace CP;


#endif