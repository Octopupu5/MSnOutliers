#ifndef CP_2025_GRAPH_H
#define CP_2025_GRAPH_H

#include "Drawable.hpp"
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
#include "config.h"

class Graph {
public:
    Graph(const std::string& title, const std::string& xLabel, const std::string& yLabel, const std::string& configPath, const std::string& outputPath)
    : _title(title), _xLabel(xLabel), _yLabel(yLabel), _configPath(configPath), _outputPath(outputPath) {}

    void addObject(const std::shared_ptr<Drawable>& obj) {
        _objects.push_back(obj);
    }

    void saveConfig() const {
        json j;
        j["title"] = _title;
        j["xLabel"] = _xLabel;
        j["yLabel"] = _yLabel;
        j["outputPath"] = _outputPath;

        j["objects"] = json::array();
        for (const auto& obj : _objects) {
            j["objects"].push_back(obj->toJson());
        }

        std::ofstream ofs(_configPath);
        ofs << std::setw(4) << j << std::endl;
    }

    void draw() const {
        std::string pyexec = std::string(SRC_PAINTINGS_DIR) + "/drawer.py";
        std::string command = std::string(PYTHON_EXEC_PATH) + " " + pyexec + " " + _configPath;
        std::cout << command << '\n';
        int ret = system(command.c_str());
        if (ret != 0) {
            std::cout << "aboba\n";
        }
    }

private:
    std::string _title;
    std::string _xLabel;
    std::string _yLabel;
    std::string _configPath;
    std::string _outputPath;
    std::vector<std::shared_ptr<Drawable>> _objects;
};

#endif  // CP_2025_GRAPH_H