#include "TheilSen.hpp"

namespace CP {
    namespace MS {
        double median(const Vec& vec);

        Vec TheilSen::compute() {
            auto n = _featuresMatrix.rows();
            auto d = _featuresMatrix.cols();
            Vec beta = Vec::Zero(d);

            for (int k = 0; k < d; ++k) {
                Vec slopes(d);
                for (int i = 0; i < n; ++i) {
                    for (int j = i+1; j < n; ++j) {
                        double dx = _featuresMatrix(j, k) - _featuresMatrix(i, k);
                        double dy = _targetMatrix(j) - _targetMatrix(i);
                        slopes(k) = (dy/dx);
                    }
                }
                beta(k) = median(slopes);
            }
            return beta;
        }
    } // namespace MS;
} // namespace CP;