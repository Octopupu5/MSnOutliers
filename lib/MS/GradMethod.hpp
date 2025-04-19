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
            GradMethod(const RData& data, double ep, double lr, const std::string& n): StatsMethod(data, n), epochs_(ep), learningRate_(lr){
                initialWeights_ = Vec::Zero(_featuresMatrix.cols());
            }
            Vec compute() override;

        protected:
            virtual double gradient(double rem) const = 0;
            virtual double loss(double rem) const = 0;
            double epochs_;
            double learningRate_;
            Vec initialWeights_;
        };
    } // namespace MS;
} // namespace CP;

#endif