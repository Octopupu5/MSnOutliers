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
            double Kernel(const Common::Row& x1, const Common::Row& x2) const;
            void DecisionBoundary(const Common::Matrix& data);
            // float _nu;
            float _gamma;
            double _rho;
            Common::Matrix _kernelMatrix;
            Common::Matrix _supportVectors;
            Common::Matrix _coefficients;
        };
    }
}
#endif // ONE_CLASS_SVM_HPP