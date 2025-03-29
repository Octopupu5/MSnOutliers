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

namespace CP {
    namespace ML {
		class Forest {
		public:
			Forest();
			Forest(uint32_t nEstimators, uint32_t depth);
			~Forest();

			void Fit(const Common::Matrix& data);
			static double H(double x) {
				return log(x) + 0.5772156;
			}
			static double C(double x) {
				return 2.0 * H(x - 1) - (2.0 * (x - 1.0) / x);
			}
			double PredictProba(const Common::Matrix& sample);
		private:
			std::vector<Node*> _trees;
			uint32_t _nEstimators;
			uint32_t _depth;

			void Clear();
			uint64_t Rand(uint64_t min, uint64_t max);
			uint32_t PathLength(Node* const tree, const Common::Matrix& sample);
			Node* CreateTree(const Common::Matrix& availableData, size_t depth);

			std::random_device _device;
			std::mt19937_64 _generator;
			std::uniform_int_distribution<uint64_t> _distribution;
		};
	}
}

#endif // IFOREST_HPP