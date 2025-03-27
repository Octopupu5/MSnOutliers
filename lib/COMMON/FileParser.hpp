#ifndef CP_BACK_FILEPARSER_H
#define CP_BACK_FILEPARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

namespace CP {
    struct RegressionUnit {
        std::vector<double> features;
        double target;
    };
    using RegressionData = std::vector<RegressionUnit>;

    class FileParser {
    public:
        RegressionData parseCSV(const std::string &path, uint32_t numFeatures);
    };
} // CP
#endif