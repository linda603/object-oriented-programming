#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
namespace fs = std::filesystem;

class Analytic {
private:
    std::string path;
    std::unordered_map<int, int> countKeys;
    std::unordered_map<int, int> sumKeys;

public:
    Analytic(std::string path) : path(path) { processFiles();}

    void processFiles() {
        for (const auto & entry : fs::directory_iterator(path)) {
            std::cout << entry.path() << std::endl;
            processData(entry.path());
        }
    }

    void processData(std::string fileName) {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Failed to open file " << path << std::endl;
        }
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ssLine(line);
            std::string key, value;

            std::getline(ssLine, key, ',');
            std::getline(ssLine, value, ',');
            countKeys[stoi(key)]++;
            sumKeys[stoi(key)] += stoi(value);
        }
    }

    void printFreqKeys() {
        for (auto it = countKeys.begin(); it != countKeys.end(); it++) {
            std::cout << "Key: " << it->first << " - frequency: " << it->second << "\n" << std::endl;
        }
    }

    void printSumKeys() {
        for (auto it = sumKeys.begin(); it != sumKeys.end(); it++) {
            std::cout << "Key: " << it->first << " - sum: " << it->second << "\n" << std::endl;
        }
    }
};

int main() {
    std::cout << "Please enter csv path: \n" << std::endl;
    std::string path;
    std::cin >> path;

    Analytic analytic(path);
    analytic.printFreqKeys();
    analytic.printSumKeys();
    return 0;
}