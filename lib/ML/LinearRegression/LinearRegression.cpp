// #include "LinearRegression.hpp"

// namespace CP {
//     namespace ML {
//         LinearRegression::LinearRegression() : _GDrunner(128, 0.0001, GD::N_ITERS, {100}, [&] (const Common::Matrix& X, const Common::Matrix& y, const Common::Matrix& w) {
//             return Common::Matrix(0);
//         }) {}

//         void LinearRegression::Fit(const Common::Matrix& X, const Common::Matrix& y) {
//             Common::Matrix toFit = X;
            
//             AddColumns(toFit, Common::Matrix(Common::Shape(X).first, Common::Row(1, 1)));
//             _GDrunner.Run(X, y);
//         }

//         Common::Matrix LinearRegression::Predict(const Common::Matrix& X) const {
//             Common::Matrix toPredict = X;
//             AddColumns(toPredict, Common::Matrix(Common::Shape(X).first, Common::Row(1, 1)));
//             return Matmul(toPredict, _GDrunner.Weights());
//         }
//     }
// }