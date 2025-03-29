#include <iostream>
#include <random>
#include "COMMON/Matrix.hpp"
#include "COMMON/DataDistortion.hpp"

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    CP::Common::Matrix X(3, 3);
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            X.At(i, j).SetValue(i * 3 + j + 1);
        }
    }

    CP::Common::DataDistortion distor(CP::Distributions::ErrorDistributions::Normal, 0.0, 1.0);

    std::cout << "Original X:" << "\n";
    X.Print();
    
    CP::Common::Matrix Normal_biased_X = distor.distortMatrix(X, gen);
    std::cout << "Distorted X:" << "\n";
    Normal_biased_X.Print();

    CP::Common::Matrix Normal_multiplied_X = distor.multiplyFeatures(X, gen);
    std::cout << "Multiplied noise X:" << "\n";
    Normal_multiplied_X.Print();

    CP::Common::Matrix Bias_const_X = distor.addConstantNoise(X, 2.0);
    std::cout << "X after adding constant:" << "\n";
    Bias_const_X.Print();

    CP::Common::Matrix Dropout_X = distor.dropoutMatrix(X, gen, 0.3);
    std::cout << "X after dropout:" << "\n";
    Dropout_X.Print();

    std::cout << "Concatenating distortions:" << "\n";
    CP::Common::Matrix concated = distor.addConstantNoise(X.ColumnMatrix(0), 0.5);
    concated.AddColumns(distor.addConstantNoise(X.ColumnMatrix(1), -0.5));
    concated.AddColumns(distor.addConstantNoise(X.ColumnMatrix(2), 10));
    concated.Print();

    return 0;
}