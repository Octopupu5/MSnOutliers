#include "iForest.hpp"

namespace CP {
    namespace ML {
		iForest::iForest() : _generator(_device()), _nEstimators(10), _depth(0) {}
		iForest::iForest(uint32_t nEstimators, uint32_t depth) : _generator(_device()), _nEstimators(nEstimators), _depth(depth) {}
		iForest::~iForest() {
			Clear();
		}

		void iForest::Fit(const Common::Matrix& data) {
			Clear();
			_trees.reserve(_nEstimators);
			for (size_t i = 0; i < _nEstimators; ++i) {
				Common::Matrix* data_cp = new Common::Matrix(data);
				_trees.push_back(CreateTree(data_cp, 0));
			}
		}

		void iForest::Clear() {
			for (Node* node : _trees) {
				std::queue<Node*> q;
				q.push(node);
				while (!q.empty()) {
					Node* current = q.front();
					q.pop();
					if (current && current->Left()) {
						q.push(current->Left());
					}
					if (current && current->Right()) {
						q.push(current->Right());
					}
					if (current) {
						delete current;
					}
				}
			}
			_trees.clear();
		}
		
		Node* iForest::CreateTree(Common::Matrix* availableData, size_t depth) {
			auto [rows, cols] = Shape(*availableData);
			if (((_depth > 0) && (depth >= _depth)) || rows == 1) {
				delete availableData;
				return NULL;
			}

			uint32_t i = Rand(0, rows - 2);
			uint32_t k = Rand(0, cols - 1);
			Common::Row featureValues;
			for (int j = 0; j < rows; ++j) {
				featureValues.push_back((*availableData)[j][k].Value());
			}
			
			std::nth_element(featureValues.begin(), featureValues.begin() + i, featureValues.end());
			Common::Feature ith_statistics = featureValues[i];
			std::nth_element(featureValues.begin(), featureValues.begin() + i + 1, featureValues.end());
			Common::Feature splitValue = (ith_statistics + featureValues[i + 1]) / 2.0;
			Node* tree = new Node(k, splitValue.Value());
			
			size_t leftSplitSize = i + 1;
			size_t rightSplitSize = rows - leftSplitSize;
			Common::Matrix* leftSplit = new Common::Matrix(leftSplitSize, Common::Row(cols));
			Common::Matrix* rightSplit = new Common::Matrix(rightSplitSize, Common::Row(cols));
			size_t leftP = 0;
			size_t rightP = 0;
			for (size_t j = 0; j < rows; ++j) {
				for (size_t m = 0; m < cols; ++m) {
					if ((*availableData)[j][k] <= splitValue && !(leftP == leftSplitSize)) {
						(*leftSplit)[leftP][m] = (*availableData)[j][m];
					}
					else {
						(*rightSplit)[rightP][m] = (*availableData)[j][m];
					}
				}
				if ((*availableData)[j][k] <= splitValue && !(leftP == leftSplitSize)) {
					++leftP;
				}
				else {
					++rightP;
				}
			}

			delete availableData;
			
			++depth;
			tree->ReplaceL(CreateTree(leftSplit, depth));
			tree->ReplaceR(CreateTree(rightSplit, depth));
			return tree;
		}
		
		uint32_t iForest::PathLength(Node* const tree, const Common::Row& sample) {
			uint32_t depth = 0.0;
			Node* currentNode = tree;
			while (currentNode) {
				int featureIndex = currentNode->FeatureIndex();
				if (sample[featureIndex].Value() < currentNode->Predicate()) {
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
		double iForest::PredictProba(const Common::Row& sample) {
			double avgPathLen = 0.0;
			for (Node* tree : _trees) {
				avgPathLen += PathLength(tree, sample);
			}
			avgPathLen /= 1.0 * _nEstimators;
			double exponent = -1.0 * (avgPathLen / C(_nEstimators));
			return pow(2, exponent);
		}

		uint64_t iForest::Rand(uint64_t min, uint64_t max) {
			std::uniform_int_distribution<uint64_t> dist(min, max);
			return dist(_generator);
		}
	}
}