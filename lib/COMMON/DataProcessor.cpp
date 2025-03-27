#include "DataProcessor.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<std::vector<double>> DataProcessor::loadCSV(const std::string& filepath) {
    std::vector<std::vector<double>> data;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string val;
        std::vector<double> row;

        while (std::getline(ss, val, ',')) {
            row.push_back(std::stod(val));
        }

        data.push_back(row);
    }

    file.close();
    return data;
}

void DataProcessor::handleMissingValues(std::vector<std::vector<double>>& data, double fillValue) {
    for (auto& row : data) {
        for (auto& val : row) {
            if (std::isnan(val)) {
                val = fillValue;
            }
        }
    }
}

void DataProcessor::removeOutliers(std::vector<std::vector<double>>& data, double threshold) {
    for (auto& col : data) {
        double mean = computeMean(col);
        double stddev = computeStdDev(col, mean);

        for (auto& val : col) {
            if (std::abs(val - mean) > threshold * stddev) {
                val = mean;
            }
        }
    }
}

double DataProcessor::computeMean(const std::vector<double>& column) {
    double sum = 0.0;
    for (const auto& val : column) {
        sum += val;
    }
    return sum / column.size();
}

double DataProcessor::computeStdDev(const std::vector<double>& column, double mean) {
    double sum = 0.0;
    for (const auto& val : column) {
        sum += (val - mean) * (val - mean);
    }
    return std::sqrt(sum / column.size());
}

void DataProcessor::normalize(std::vector<std::vector<double>>& data) {
    for (auto& col : data) {
        double mean = computeMean(col);
        double stddev = computeStdDev(col, mean);

        for (auto& val : col) {
            val = (val - mean) / stddev;
        }
    }
}

void DataProcessor::splitData(const std::vector<std::vector<double>>& data, double trainRatio,
                              std::vector<std::vector<double>>& trainData, std::vector<std::vector<double>>& testData) {
    size_t train = static_cast<size_t>(data.size() * trainRatio);
    trainData.assign(data.begin(), data.begin() + train);
    testData.assign(data.begin() + train, data.end());
}

void DataProcessor::addOutliers(std::vector<std::vector<double>>& data, double percentage, double scale) {
    size_t outliers = static_cast<size_t>(data.size() * percentage);
    for (size_t i = 0; i < outliers; ++i) {
        size_t row = rand() % data.size();
        size_t col = rand() % data[row].size();
        data[row][col] *= scale;
    }
}