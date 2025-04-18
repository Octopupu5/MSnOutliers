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
            Tukey(const RData& data, double del, double eps, double lr): GradMethod(data, eps, lr){
                auto n = _targetMatrix.size();
                if (n > 1) {
                    double avg = _targetMatrix.sum()/n;
                    double s_square = 0;
                    for (int i = 0; i < n; ++i) {
                        s_square += (_targetMatrix(i) - avg) * (_targetMatrix(i) - avg);
                    }
                    delta_ = s_square/(n-1);
                } else {
                    delta_ = del;
                }
            }
        private:
            double gradient(double rem) const override;
            double loss(double rem) const override;
            double delta_;
        };
    } // namespace MS;
} // namespace CP;

#endif
