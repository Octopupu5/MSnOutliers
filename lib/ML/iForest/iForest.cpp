#include "iForest.hpp"

namespace CP {
    namespace ML {
		iForest::iForest(uint32_t nEstimators, uint32_t depth, const Common::Matrix& data) : _generator(_device()), _nEstimators(nEstimators), _depth(depth), _data(data) {
			auto [rows, cols] = Common::Shape(_data);
			_dataRows = rows;
			_dataCols = cols;
		}
		iForest::~iForest() {
			Clear();
		}

		void iForest::Fit() {
			Clear();
			_trees.reserve(_nEstimators);
			
			for (size_t i = 0; i < _nEstimators; ++i) {
				std::vector<size_t>* indices = new std::vector<size_t>(_dataRows);
				for (size_t j = 0; j < _dataRows; ++j) {
					(*indices)[j] = j;
				}
				std::shuffle((*indices).begin(), (*indices).end(), _generator);
				size_t subsetSize = std::min(_dataRows, static_cast<size_t>(256));
				(*indices).resize(subsetSize);
				
				_trees.push_back(CreateTree(indices, 0));
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
		
		Node* iForest::CreateTree(std::vector<size_t>* indices, size_t depth) {
			size_t rows = (*indices).size();
			if (((_depth > 0) && (depth >= _depth)) || rows == 1) {
				delete indices;
				return NULL;
			}

			uint32_t i = Rand(0, rows - 2);
			uint32_t k = Rand(0, _dataCols - 1);
			Common::Row featureValues(rows);
			size_t j = 0;
			for (size_t idx : (*indices)) {
				featureValues[j] = _data[idx][k];
				++j;
			}
			
			std::nth_element(featureValues.begin(), featureValues.begin() + i, featureValues.end());
			double ith_statistics = featureValues[i];
			std::nth_element(featureValues.begin(), featureValues.begin() + i + 1, featureValues.end());
			double splitValue = (ith_statistics + featureValues[i + 1]) / 2.0;
			Node* tree = new Node(k, splitValue);
			
			size_t leftSplitSize = i + 1;
			size_t rightSplitSize = rows - leftSplitSize;
			size_t leftP = 0;
			size_t rightP = 0;
			std::vector<size_t>* leftIndices = new std::vector<size_t>(leftSplitSize);
			std::vector<size_t>* rightIndices = new std::vector<size_t>(rightSplitSize);
			for (size_t idx : (*indices)) {
				if (_data[idx][k] <= splitValue && !(leftP == leftSplitSize)) {
					(*leftIndices)[leftP] = idx;
					++leftP;
				}
				else {
					(*rightIndices)[rightP] = idx;
					++rightP;
				}
			}

			delete indices;

			++depth;
			tree->ReplaceL(CreateTree(leftIndices, depth));
			tree->ReplaceR(CreateTree(rightIndices, depth));
			return tree;
		}
		
		uint32_t iForest::PathLength(Node* const tree, const Common::Row& sample) {
			uint32_t depth = 0.0;
			Node* currentNode = tree;
			while (currentNode) {
				size_t featureIndex = currentNode->FeatureIndex();
				if (sample[featureIndex] < currentNode->Predicate()) {
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
			return pow(2, -1.0 * (avgPathLen / (2.0 * (log(_nEstimators - 1) + 0.5772156 - (_nEstimators - 1.0) / _nEstimators)) / _nEstimators));
		}

		uint64_t iForest::Rand(uint64_t min, uint64_t max) {
			std::uniform_int_distribution<uint64_t> dist(min, max);
			return dist(_generator);
		}
	}
}