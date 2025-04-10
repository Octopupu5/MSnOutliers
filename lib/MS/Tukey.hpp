#include "GradMethod.hpp"
#ifndef CP_2025_TUKEY_H
#define CP_2025_TUKEY_H

namespace CP {
    namespace MS {
        using RData = Common::RegressionData;
        using Vec = Eigen::VectorXd;
        using Mat = Eigen::MatrixXd;

        class Tukey : public GradMethod {
        public:
            Tukey(const RData& data, double del, double eps, double lr): GradMethod(data, eps, lr), delta_(del){}
        private:
            double gradient(double rem) const override;
            double loss(double rem) const override;
            double delta_;
        };
    } // namespace MS;
} // namespace CP;

#endif
