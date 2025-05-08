#ifndef KNN_OUTLIER_HPP
#define KNN_OUTLIER_HPP

#include "../../COMMON/Matrix.hpp"
#include <algorithm>
#include <vector>
#include <cmath>

namespace CP {
    namespace ML {
        class KNN {
        public:
            KNN(size_t k = 10, double contamination = 0.01);

            void Fit(const Common::Matrix& data);
            Common::Matrix Predict(const Common::Matrix& data) const;
            double Threshold() const;
            std::vector<std::pair<double, size_t>>& PairDistances();
        private:
            size_t _k;
            double _contamination;
            std::vector<std::pair<double, size_t>> _sortedDistances;
            Common::Matrix _train;
            double _threshold;
        };
    }
}
#endif // KNN_OUTLIER_HPP