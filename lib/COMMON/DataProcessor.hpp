#ifndef DATA_PROCESSOR_HPP
#define DATA_PROCESSOR_HPP

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>

class DataProcessor {
public:
    DataProcessor() = default;

    void handleMissingValues(std::vector<std::vector<double>>& data, double fillValue = 0.0);
    void removeOutliers(std::vector<std::vector<double>>& data, double threshold = 3.0);
    void normalize(std::vector<std::vector<double>>& data);
    void splitData(const std::vector<std::vector<double>>& data, double trainRatio,
                   std::vector<std::vector<double>>& trainData, std::vector<std::vector<double>>& testData);
    void addOutliers(std::vector<std::vector<double>>& data, double percentage, double scale = 10.0);

private:
    double computeMean(const std::vector<double>& column);
    double computeStdDev(const std::vector<double>& column, double mean);
};

#endif // DATA_PROCESSOR_HPP
