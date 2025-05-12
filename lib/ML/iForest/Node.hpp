#ifndef NODE_HPP
#define NODE_HPP

#include <inttypes.h>

namespace CP {
	namespace ML {
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

			Node* _subL;
			Node* _subR;
		};
	}
}

#endif // NODE_HPP