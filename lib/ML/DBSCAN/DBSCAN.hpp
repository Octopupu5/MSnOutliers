#ifndef DBSCAN_HPP
#define DBSCAN_HPP

#include "../../COMMON/Matrix.hpp"
#include <vector>

namespace CP {
    namespace ML {
        class DBSCAN {
        public:
            DBSCAN(double r, uint32_t minimumClusterSize);
            void Fit(const Common::Matrix& data);

        private:
            std::vector<uint32_t> GetNeighbors(uint32_t index) const;
            void ExpandCluster(const std::vector<uint32_t>& neighbors, uint32_t index, uint32_t clusterId);

            double _r;
            uint32_t _minimumClusterSize;
            std::vector<uint32_t> _idToCluster;
            Common::Matrix _data;
        };
    }
}

#endif // DBSCAN_HPP