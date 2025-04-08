#ifndef LINEARREGRESSION_HPP
#define LINEARREGRESSION_HPP

#include "../../GD/Adam.hpp"

namespace CP {
    namespace ML {
        class LinearRegression {
        public:
            LinearRegression();
            void Fit(const Common::Matrix& X, const Common::Matrix& y);
            Common::Matrix Predict(const Common::Matrix& X) const;
        private:
            GD::Adam _GDrunner;
        };
    }
}

#endif // LINEARREGRESSION_HPP