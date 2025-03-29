#ifndef ONE_CLASS_SVM_HPP
#define ONE_CLASS_SVM_HPP

#include "../../COMMON/Matrix.hpp"

namespace CP {
    namespace ML {
        class OneClassSVM {
        public:
            OneClassSVM(float nu, float gamma);
            void Fit(const Common::Matrix& data);

            // 1 = anomaly; 0 = inlier
            Common::Matrix Predict(const Common::Matrix& data) const;

        private:
            // radial basis func kernel
            double Kernel(const Common::Matrix& x1, const Common::Matrix& x2) const;

            // decision boundary offset (rho)
            void DecisionBoundary(const Common::Matrix& data);
            // regularization
            float nu;
            // kernel coef
            float gamma;
            Common::Matrix supportVectors;
            // coefs for supvectors
            Common::Matrix coefficients;
            // offset for decision boundary
            double rho;
        };
    }
}
#endif // ONE_CLASS_SVM_HPP