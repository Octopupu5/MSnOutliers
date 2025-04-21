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
            GradMethod(const RData& data, const std::string& n, double delta, double ep, double lr): StatsMethod(data, n){
                initialWeights_ = Vec::Zero(_featuresMatrix.cols());
            }
            Vec compute() override;

        protected:
            virtual double gradient(double rem) const = 0;
            virtual double loss(double rem) const = 0;
            Vec initialWeights_;
        };
    } // namespace MS;
} // namespace CP;

#endif