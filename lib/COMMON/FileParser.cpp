#include "FileParser.hpp"
#include <sstream>

namespace CP {
    namespace Common {
        using RData = RegressionData;

        RData FileParser::parseCSV(const std::string &path, uint32_t numFeatures) {
            std::ifstream in(path);
            if (!in.is_open()) {
                std::cout << "Error opening the file!\n";
                exit(0);
            }
            RData res;
            std::string s;
            while (std::getline(in, s)) {
                std::vector<double> features(numFeatures);
                double target;
                std::string feat;
                std::stringstream ss(s);
                uint32_t featuresCount = 0;
                while (std::getline(ss, feat, ';')) {
                    if (featuresCount < numFeatures) {
                        features[featuresCount] = std::stod(feat);
                        ++featuresCount;
                    } else {
                        target = std::stod(feat);
                    }
                }
                res.push_back({features, target});
            }
            return res;
        }
    }
}