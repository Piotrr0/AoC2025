#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

struct Position {
    int x;
    int y;
    int z;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

Position parseLine(const std::string& line) {
    Position pos;
    char comma;

    std::stringstream ss(line);    
    ss >> pos.x >> comma >> pos.y >> comma >> pos.z;
    return pos;
}

std::vector<Position> readDataFromFile(const std::string& path) {
    std::vector<Position> poses;
    std::ifstream dataFile(path);

    if (!dataFile.is_open())
        return {};

    std::string line;
    while (std::getline(dataFile, line)) {
        poses.emplace_back(parseLine(line));
    }

    dataFile.close();
    return poses;
}

float getDistance(const Position& A, const Position& B) {
    long long dx = A.x - B.x;
    long long dy = A.y - B.y;
    long long dz = A.z - B.z;

    long long dx2 = dx * dx;
    long long dy2 = dy * dy;
    long long dz2 = dz * dz;

    long long sumOfSquares = dx2 + dy2 + dz2;
    return std::sqrt(sumOfSquares);
}

struct PositionPair {
    Position a;
    Position b;
    float distance;
};

void getCombinations(std::vector<PositionPair>& out, const std::vector<Position>& positions, std::vector<Position>& current, int start_index) {
    if (current.size() == 2) {
        float dist = getDistance(current[0], current[1]);
        out.push_back({current[0], current[1], dist});
        return;
    }

    for (int i = start_index; i < positions.size(); i++) {
        current.emplace_back(positions[i]);
        getCombinations(out, positions, current, i + 1);
        current.pop_back();
    }
}

int getLargestCircuitSizeProduct(const std::vector<PositionPair>& pairs, std::vector<std::vector<Position>> circuits, int sizeThreshold = 3, int connectionsLimit = 1000) {
    for (int i = 0; i < connectionsLimit && i < pairs.size(); i++) {
        int circuitA = -1;
        int circuitB = -1;
        
        for (int j = 0; j < circuits.size(); j++) {
            for (const auto& pos : circuits[j]) 
            {
                if (pos == pairs[i].a) 
                    circuitA = j;
                if (pos == pairs[i].b) 
                    circuitB = j;
            }

            if (circuitA != -1 && circuitB != -1) 
                break;
        }

        if (circuitA == circuitB) {
            continue;
        }

        for (const auto& pos : circuits[circuitB]) {
            circuits[circuitA].push_back(pos);
        }

        circuits[circuitB] = circuits.back();
        circuits.pop_back();
    }

    std::vector<int> sizes;
    for (const auto& circuit : circuits) {
        sizes.emplace_back(circuit.size());
    }

    std::sort(sizes.begin(), sizes.end(), std::greater<int>());
    if (sizes.size() >= sizeThreshold) {
        return sizes[0] * sizes[1] * sizes[2];
    }

    return 0;
}

int main()
{
    std::vector<Position> positions = readDataFromFile("../data.txt");

    std::vector<PositionPair> pairs;
    std::vector<Position> buffer;
    getCombinations(pairs, positions, buffer, 0);
    std::sort(pairs.begin(), pairs.end(), [](const PositionPair& a, const PositionPair& b) {
        return a.distance < b.distance;
    });

    std::vector<std::vector<Position>> circuits;
    for (const auto& pos : positions) {
        circuits.push_back({pos});
    }

    std::cout << getLargestCircuitSizeProduct(pairs, circuits) << std::endl;
    return 0;
}