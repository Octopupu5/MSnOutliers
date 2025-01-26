#include "Node.hpp"

#define UNKNOWN 1000000

Node::Node(): _j(UNKNOWN), _predicate(0), _sub_l(NULL), _sub_r(NULL) {};
Node::Node(const uint64_t& feature, double predicate) :
		_j(feature), _predicate(predicate),
		_sub_l(NULL), _sub_r(NULL) {}

Node::~Node() {
	ReplaceL(nullptr);
	ReplaceR(nullptr);
}

uint64_t Node::FeatureIndex() const {
	return _j;
};

double Node::Predicate() const {
	return _predicate;
};

Node* Node::Left() const {
	return _sub_l;
};

Node* Node::Right() const {
	return _sub_r;
};

void Node::ReplaceL(Node* node) {
	if (_sub_l) {
		delete _sub_l;
	}
	_sub_l = node;
}

void Node::ReplaceR(Node* node) {
	if (_sub_r) {
		delete _sub_r;
	}
	_sub_r = node;
}