#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

std::unordered_map<std::string, std::vector<std::string>> getDataFromFile(const std::string& path) {
    std::unordered_map<std::string, std::vector<std::string>> nodes;
    std::ifstream f(path);
    
    if (!f.is_open()) {
        return {};
    }

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue; 

        std::stringstream ss(line);
        std::string parent;
        std::vector<std::string> children;

        ss >> parent;
        if (parent.back() == ':') {
            parent.pop_back();
        }

        std::string child;
        while (ss >> child) {
            children.emplace_back(child);
        }
        nodes[parent] = children;
    }

    f.close();
    return nodes;
}

int findAllPaths(const std::string& root, 
                std::unordered_set<std::string>& visited, 
                std::unordered_map<std::string, std::vector<std::string>>& nodes) 
{    
    if (root == "out")
        return 1;

    if (visited.find(root) != visited.end()) 
        return 0;

    if (nodes.find(root) == nodes.end())
        return 0;

    visited.insert(root);

    int paths = 0;
    for (const auto& child : nodes[root]) {
        paths += findAllPaths(child, visited, nodes);
    }
    visited.erase(root);
    
    return paths;
}

long long findAllPaths2(const std::string& root, bool fft, bool dac, 
                        std::unordered_map<std::string, std::vector<std::string>>& nodes, 
                        std::unordered_map<std::string, long long>& memo) 
{
    if (root == "out") {
        if (fft && dac)
            return 1;
        
        return 0;
    }

    if (nodes.find(root) == nodes.end()) 
        return 0;

    std::string fftStr = fft ? "true" : "false";
    std::string dacStr = dac ? "true" : "false";
    std::string key = root + fftStr + dacStr;

    if (memo.find(key) != memo.end())
        return memo[key];

    fft = fft || root == "fft";
    dac = dac || root == "dac";

    long long paths = 0;
    for (const auto& child : nodes[root]) {
        paths += findAllPaths2(child, fft, dac, nodes, memo);
    }
    memo[key] = paths;

    return paths;
}

int main() {
    auto nodes = getDataFromFile("../data.txt");

    std::unordered_set<std::string> visited;
    std::cout << findAllPaths("you", visited, nodes) << std::endl;
    
    std::unordered_map<std::string, long long> memo;
    std::cout << findAllPaths2("svr", false, false, nodes, memo) << std::endl;

    return 0;
}