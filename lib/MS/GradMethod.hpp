#include "StatsMethod.hpp"
#ifndef CP_2025_GRAD_METHOD_H
#define CP_2025_GRAD_METHOD_H

namespace CP {
    namespace MS {
        using RData = Common::RegressionData;
        using Vec = Eigen::VectorXd;
        using Mat = Eigen::MatrixXd;

        double median(const Vec& vec);

        class GradMethod : public StatsMethod {
        public:
            GradMethod(const RData& data, double ep, double lr): StatsMethod(data), epochs_(ep), learningRate_(lr){}
            Vec compute() override;
        private:
            virtual double gradient(double rem) const = 0;
            virtual double loss(double rem) const = 0;
            double epochs_;
            double learningRate_;
        };
    } // namespace MS;
} // namespace CP;

#endif