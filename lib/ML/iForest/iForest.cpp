#include "iForest.hpp"

Forest::Forest() : _generator(_device()), _nEstimators(10), _depth(0) {}
Forest::Forest(uint32_t nEstimators, uint32_t depth) : _generator(_device()), _nEstimators(nEstimators), _depth(depth) {}
Forest::~Forest() {
	Clear();
}

void Forest::Fit(const Matrix& data) {
	Clear();
	_trees.reserve(_nEstimators);
	for (size_t i = 0; i < _nEstimators; ++i) {
		_trees.push_back(CreateTree(data, 0));
	}
}

void Forest::Clear() {
	for (Node* node : _trees) {
		if (node) {
			delete node;
		}
	}
	_trees.clear();
}

uint64_t Forest::Rand(uint64_t min, uint64_t max) {
	return min + (_distribution(_generator) % (max - min + 1));
}

Node* Forest::CreateTree(const Matrix& availableData, size_t depth) {
	if (((_depth > 0) && (depth >= _depth)) || availableData.Rows() == 1) {
		return NULL;
	}

	uint32_t i = Rand(0, availableData.Rows() - 1);
	uint32_t k = Rand(0, availableData.Cols() - 1);
	std::vector<double> featureValues;
	for (int j = 0; j < availableData.Rows(); ++j) {
		featureValues.push_back(availableData.At(j, k).Value());
	}
	std::nth_element(featureValues.begin(), featureValues.begin() + i, featureValues.end());
	double ith_statistics = featureValues[i];
	std::nth_element(featureValues.begin(), featureValues.begin() + i + 1, featureValues.end());
	double ith_next_statistics = featureValues[i + 1];
	double splitValue = (ith_statistics + ith_next_statistics) / 2.0;

	Node* tree = new Node(k, splitValue);
		
	std::vector<std::vector<CP::Common::Feature>> leftSplit;
	std::vector<std::vector<CP::Common::Feature>> rightSplit;
	for (int j = 0; j < availableData.Rows(); ++j) {
		if (availableData.At(j, k).Value() <= splitValue) {
			leftSplit.push_back(availableData[j]);
		}
		else {
			rightSplit.push_back(availableData[j]);
		}
	}

	++depth;
	tree->ReplaceL(CreateTree(leftSplit, depth));
	tree->ReplaceR(CreateTree(rightSplit, depth));
	return tree;
}

	uint32_t Forest::PathLength(Node* const tree, const Matrix& sample) {
		uint32_t depth = 0.0;
		Node* currentNode = tree;
		while (currentNode) {
			int featureIndex = currentNode->FeatureIndex();
			if (sample.At(0, featureIndex).Value() < currentNode->Predicate()) {
				currentNode = currentNode->Left();
			}
			else {
				currentNode = currentNode->Right();
			}
			++depth;
		}
		return depth;
	}

	// 1 = anomaly / 0 = inlier
	double Forest::PredictProba(const Matrix& sample) {
		double avgPathLen = 0.0;
		for (Node* tree : _trees) {
			avgPathLen += PathLength(tree, sample);
		}
		avgPathLen /= 1.0 * _nEstimators;
		double exponent = -1.0 * (avgPathLen / C(_nEstimators));
		return pow(2, exponent);
    }