#include "ML/OCSVM/OneClassSVM.hpp"

int main() {    
    int rows = 100;
    int cols = 100;
    CP::Common::Matrix data = CP::Common::Matrix(rows, CP::Common::Row(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] = CP::Common::Feature(i + j + (i % 10)); 
        }
    }
    CP::Common::Row arb_row = data[25];
    // double nu = 0.05;
    double gamma = 1.0 / Shape(data).second;
    // CP::ML::OneClassSVM svm(nu, gamma);
    CP::ML::OneClassSVM svm(gamma);
    svm.Fit(data);
    
    CP::Common::Matrix testSample = CP::Common::Matrix({
        {10.0, 20.0, 30.0, 40.0, 500.0, 10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0,
        10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0,
        10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0,
        10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0,
        10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0, 10.0, 20.0, 30.0, 40.0, 50.0},
    });
    testSample.push_back(arb_row);
    // 1 = anomaly; 0 = inlier
    CP::Common::Matrix answers = svm.Predict(testSample);
    Print(answers);
    return 0;
}