#ifndef CP_2025_HUBER_H
#define CP_2025_HUBER_H
#include "GradMethod.hpp"

namespace CP {
    namespace MS {
        using RData = Common::RegressionData;
        using Mat = Eigen::MatrixXd;
        using Vec = Eigen::VectorXd;

        class Huber : public GradMethod {
            public:
                Huber(const RData& data, double del, double eps, double lr): GradMethod(data, eps, lr), delta_(del){}
                
            private:
                double loss(double residual) const override;
                double gradient(double residual) const override;
                double delta_;
        };
    } // namespace MS;
} // namespace CP;

#endif
