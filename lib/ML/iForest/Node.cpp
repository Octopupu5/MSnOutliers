#include "Node.hpp"
namespace CP {
	namespace ML {
		Node::Node(const uint64_t& feature, double predicate) : _j(feature), _predicate(predicate), _subL(nullptr), _subR(nullptr) {}

		Node::~Node() = default;
		uint64_t Node::FeatureIndex() const {
			return _j;
		};

		double Node::Predicate() const {
			return _predicate;
		};

		Node* Node::Left() const {
			return _subL;
		};

		Node* Node::Right() const {
			return _subR;
		};

		void Node::ReplaceL(Node* node) {
			if (_subL) {
				delete _subL;
			}
			_subL = node;
		}

		void Node::ReplaceR(Node* node) {
			if (_subR) {
				delete _subL;
			}
			_subR = node;
		}
	}
}