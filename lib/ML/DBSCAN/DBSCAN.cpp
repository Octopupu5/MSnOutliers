#include "DBSCAN.hpp"
#include <cmath>
#include <unordered_set>

namespace CP {
    namespace ML {
        DBSCAN::DBSCAN(double r, uint32_t minimumClusterSize) : _r(r), _minimumClusterSize(minimumClusterSize) {}

        std::vector<uint32_t> DBSCAN::GetNeighbors(uint32_t index) const {
            std::vector<uint32_t> neighbors;
            for (size_t i = 0; i < _data.Rows(); ++i) {
                if (_data.RowMatrix(index).L2Norm(_data.RowMatrix(i)) <= _r) {
                    neighbors.push_back(i);
                }
            }
            return neighbors;
        }

        void DBSCAN::ExpandCluster(const std::vector<uint32_t>& neighbors, uint32_t index, uint32_t clusterId) {
            _idToCluster[index] = clusterId;
            std::unordered_set<uint32_t> seeds(neighbors.begin(), neighbors.end());

            while (!seeds.empty()) {
                uint32_t current = *seeds.begin();
                seeds.erase(seeds.begin());

                if (_idToCluster[current] == 0) {
                    _idToCluster[current] = clusterId;
                }

                if (_idToCluster[current] != -1) {
                    continue;
                }

                _idToCluster[current] = clusterId;
                std::vector<uint32_t> currentNeighbors = GetNeighbors(current);

                if (currentNeighbors.size() >= _minimumClusterSize) {
                    seeds.insert(currentNeighbors.begin(), currentNeighbors.end());
                }
            }
        }

        void DBSCAN::Fit(const Common::Matrix& data) {
            _data = data;
            
            int clusterId = 0;
            _idToCluster = std::vector<uint32_t> (data.Rows(), -1);
            for (int i = 0; i < data.Rows(); ++i) {
                if (_idToCluster[i] != -1) {
                    continue;
                }

                std::vector<uint32_t> neighbors = GetNeighbors(i);
                if (neighbors.size() < _minimumClusterSize) {
                    _idToCluster[i] = 0;
                    continue;
                }

                ++clusterId;
                ExpandCluster(neighbors, i, clusterId);
            }
        }
    }
}