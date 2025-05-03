#ifndef ONE_CLASS_SVM_HPP
#define ONE_CLASS_SVM_HPP

#include "../../COMMON/Matrix.hpp"

namespace CP {
    namespace ML {
        class OneClassSVM {
        public:
            // OneClassSVM(float nu, float gamma);
            OneClassSVM(float gamma);
            void Fit(const Common::Matrix& data);

            // 1 = anomaly; 0 = inlier
            Common::Matrix Predict(const Common::Matrix& data) const;

        private:
            double Kernel(const Common::Matrix& x1, const Common::Matrix& x2) const;
            void DecisionBoundary(const Common::Matrix& data);
            // float _nu;
            float _gamma;
            double _rho;
            Common::Matrix _supportVectors;
            Common::Matrix _coefficients;
        };
    }
}
#endif // ONE_CLASS_SVM_HPP