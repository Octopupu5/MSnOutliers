#include "ML/iForest/iForest.hpp"
#include "COMMON/Matrix.hpp"
using namespace CP::Common;

int main() {    
    int rows = 100;
    int cols = 100;
    CP::Common::Matrix data = CP::Common::Matrix(rows, CP::Common::Row(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] = CP::Common::Feature(i + j + (i % 10)); 
        }
    }
    CP::Common::Row arb_row = data[1];

    // for (int i = 0; i < 100; ++i) {
    //     data.At(i, 0) = CP::Common::Feature(1000 + i); 
    // }
    
    CP::ML::iForest forest(1000, 10); 
    forest.Fit(data);
    
    CP::Common::Matrix testSample = CP::Common::Matrix({
        {1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0,
        1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0,
        1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0,
        1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0,
        1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0},
    });
    testSample.push_back(arb_row);

    for (size_t i = 0; i < 2; ++i) {
        CP::Common::Row sample = testSample[i];
        double proba = forest.PredictProba(sample);
        std::cout << "Sample " << i << " anomaly probability: " << proba << std::endl;
    }

    return 0;
}