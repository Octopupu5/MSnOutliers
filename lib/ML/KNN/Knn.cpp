#include "Knn.hpp"
#include <queue>
#include <limits>

namespace CP {
    namespace ML {

        KNN::KNN(size_t k, double contamination) : _k(k), _contamination(contamination), _threshold(0.0) {}

        void KNN::Fit(const Common::Matrix& data) {
            _train = data;
            size_t n = data.size();
            _sortedDistances = std::vector<std::pair<double, size_t>>(n);

            for (size_t i = 0; i < n; ++i) {
                std::vector<double> distances;
                distances.reserve(n - 1);
                for (size_t j = 0; j < n; ++j) {
                    if (i == j) {
                        continue;
                    }
                    distances.push_back(L2Norm(data[i], data[j]));
                }
                std::nth_element(distances.begin(), distances.begin() + _k - 1, distances.end());
                _sortedDistances[i] = {distances[_k - 1], i};
            }

            std::nth_element(_sortedDistances.begin(), _sortedDistances.begin() + n * (1.0 - _contamination), _sortedDistances.end());
            _threshold = _sortedDistances[n * (1.0 - _contamination)].first;
        }

        Common::Matrix KNN::Predict(const Common::Matrix& data) const {
            size_t n = data.size();
            Common::Matrix out(n, Common::Row(1, 0.0));
            for (size_t i = 0; i < n; ++i) {
                std::vector<double> distances;
                distances.reserve(_train.size());
                for (const auto& x : _train) {
                    distances.push_back(L2Norm(x, data[i]));
                }
                std::nth_element(distances.begin(), distances.begin() + _k - 1, distances.end());
                double kdist = distances[_k - 1];
                out[i][0] = (kdist >= _threshold) ? 1.0 : 0.0;
            }
            return out;
        }

        double KNN::Threshold() const {
            return _threshold;
        }

        std::vector<std::pair<double, size_t>>& KNN::PairDistances() {
            return _sortedDistances;
        }
    }
}