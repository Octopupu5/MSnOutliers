#ifndef IFOREST_HPP
#define IFOREST_HPP

#include "../../COMMON/Matrix.hpp"
#include "Node.hpp"
#include <map>
#include <random>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

namespace CP {
    namespace ML {
		class iForest {
		public:
			iForest(uint32_t nEstimators, uint32_t depth, const Common::Matrix& data);
			~iForest();

			void Fit();
			double PredictProba(const Common::Row& sample);
		private:
			const Common::Matrix& _data;
			size_t _dataRows;
			size_t _dataCols;
			std::vector<Node*> _trees;
			uint32_t _nEstimators;
			uint32_t _depth;

			void Clear();
			uint64_t Rand(uint64_t min, uint64_t max);
			uint32_t PathLength(Node* const tree, const Common::Row& sample);
			Node* CreateTree(std::vector<size_t>* indices, size_t depth);
			
			std::random_device _device;
			std::mt19937_64 _generator;
		};
	}
}

#endif // IFOREST_HPP