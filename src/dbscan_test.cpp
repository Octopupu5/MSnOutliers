#include "ML/DBSCAN/DBSCAN.hpp"

int main() {    
    int rows = 100;
    int cols = 100;
    CP::Common::Matrix data = CP::Common::Matrix(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data.At(i, j) = CP::Common::Feature(i + j + (i % 10)); 
        }
    }
    CP::Common::Matrix arb_row = data.RowMatrix(1);

    for (int i = 0; i < 10; ++i) {
        data.At(i, 0) = CP::Common::Feature(1000 + i); 
    }
    
    CP::ML::DBSCAN dbscan(40, 5); 
    dbscan.Fit(data);
    std::vector<int32_t> idToCluster = dbscan.getIdToCluster();
    for (int i = 0; i < 100; ++i) {
        std::cout << idToCluster[i] << " ";
    }

    return 0;
}