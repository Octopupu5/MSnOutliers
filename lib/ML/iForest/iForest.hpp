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

#define H(i) (log(i) + 0.5772156649)
#define C(n) (2 * H(n - 1) - (2 * (n - 1) / n))

class Forest {
public:
	Forest();
	Forest(uint32_t nEstimators, uint32_t depth);
	~Forest();

	void Fit(const Matrix& data);
    double PredictProba(const Matrix& sample);
private:
	std::vector<Node*> _trees;
	uint32_t _nEstimators;
	uint32_t _depth;

	void Clear();
	uint64_t Rand(uint64_t min, uint64_t max);
	uint32_t PathLength(Node* const tree, const Matrix& sample);
	Node* CreateTree(const Matrix& availableData, size_t depth);

	std::random_device _device;
	std::mt19937_64 _generator;
	std::uniform_int_distribution<uint64_t> _distribution;
};


#endif // IFOREST_HPP