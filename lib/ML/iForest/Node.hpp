#ifndef NODE_HPP
#define NODE_HPP

#include "Feature.hpp"
#include <inttypes.h>

class Node {
public:
	Node();
	Node(const uint64_t& feature, double predicate);
	~Node();

	uint64_t FeatureIndex() const;
	double Predicate() const;

	Node* Left() const;
	Node* Right() const;

	void ReplaceL(Node* node);
	void ReplaceR(Node* node);

private:
	uint64_t _j;
	double _predicate;

	Node* _sub_l;
	Node* _sub_r;
};

#endif // NODE_HPP