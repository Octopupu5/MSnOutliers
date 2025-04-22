#include "TheilSen.hpp"

namespace CP {
    namespace MS {
        double median(const Vec& vec);

        Vec TheilSen::compute() {
            auto n = _featuresMatrix.rows();
            auto d = _featuresMatrix.cols();
            std::vector<Vec> slope_vectors;
        
            for (int i = 0; i < n; ++i) {
                for (int j = i + 1; j < n; ++j) {
                    Vec dx = _featuresMatrix.row(j) - _featuresMatrix.row(i);
                    double dy = _targetMatrix(j) - _targetMatrix(i);
                    if ((dx.array().abs() < 1e-12).all()) continue;
        
                    Vec s = dx.unaryExpr([](double val) {
                        return std::abs(val) < 1e-12 ? std::numeric_limits<double>::quiet_NaN() : val;
                    });
                    s = dy * s.cwiseInverse();
                    slope_vectors.push_back(s);
                }
            }
        
            Eigen::MatrixXd S(slope_vectors.size(), d);
            for (size_t i = 0; i < slope_vectors.size(); ++i) {
                S.row(i) = slope_vectors[i];
            }
        
            Vec beta(d);
            for (int k = 0; k < d; ++k) {
                std::vector<double> col;
                for (int i = 0; i < S.rows(); ++i) {
                    double val = S(i, k);
                    if (!std::isnan(val)) col.push_back(val);
                }
                Vec col_vec = Eigen::Map<Vec>(col.data(), col.size());
                beta(k) = col.empty() ? 0.0 : median(col_vec);
            }
        
            return beta;
        }
    } // namespace MS;
} // namespace CP;