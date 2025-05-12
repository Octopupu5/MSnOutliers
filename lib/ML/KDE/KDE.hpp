#ifndef KERNEL_DENSITY_ESTIMATOR_HPP
#define KERNEL_DENSITY_ESTIMATOR_HPP

#include "../../COMMON/Matrix.hpp"

namespace CP {
    namespace ML {
        class KernelDensityEstimator {
        public:
            KernelDensityEstimator(float gamma);
            void Fit(const Common::Matrix& data);
            // 1 = anomaly; 0 = inlier
            Common::Matrix Predict(const Common::Matrix& data) const;
        private:
            double Kernel(const Common::Row& x1, const Common::Row& x2) const;
            void FindThreshold();
            float _gamma;
            double _rho;
            Common::Matrix _data;
            Common::Matrix _kernelMatrix;
        };
    }
}

#endif // KERNEL_DENSITY_ESTIMATOR_HPP