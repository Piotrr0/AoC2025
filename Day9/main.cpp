#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

struct Position {
    int x;
    int y;
};

Position parseLine(const std::string& line) {
    Position pos;
    char comma;

    std::stringstream ss(line);    
    ss >> pos.x >> comma >> pos.y;
    return pos;
}

std::vector<Position> readDataFromFile(const std::string& path) {
    std::vector<Position> positions;
    std::ifstream dataFile(path);

    if (!dataFile.is_open())
        return {};

    std::string line;
    while (std::getline(dataFile, line)) {
        positions.emplace_back(parseLine(line));
    }

    dataFile.close();
    return positions;
}

long long findLargestRectangle(const std::vector<Position>& positions) {
    long long out = -1;

    auto getArea = [](const Position& A, const Position B) -> long long {
        long long width = std::abs(A.x - B.x) + 1;
        long long height = std::abs(A.y - B.y) + 1;
        return width * height;
    };

    for (int i = 0; i<positions.size(); i++)
    {
        for (int j = i + 1; j < positions.size(); j++) {
            out = std::max(getArea(positions[i], positions[j]), out);
        }
    }
    return out;
}

std::vector<std::vector<int>> getScoreboard(std::vector<Position> positions) {
    std::sort(positions.begin(), positions.end(), [](const Position& a, const Position& b) {
        if (a.y != b.y) return a.y < b.y;
        return a.x < b.x;
    });

    std::vector<std::vector<int>> scores(20, std::vector<int>(20, 0));
    for (const auto& pos : positions) {
        int x = pos.x;
        int y = pos.y;
        if (x >= 0 && x < 20 && y >= 0 && y < 20) {
            scores[y][x]++; 
        }
    }

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
            int top = (y > 0) ? scores[y - 1][x] : 0;
            int left = (x > 0) ? scores[y][x - 1] : 0;
            int topLeft = (y > 0 && x > 0) ? scores[y - 1][x - 1] : 0;

            scores[y][x] += top + left - topLeft;
        }
    }
    
    return scores;
}

bool isValidRectangle(const Position& A, const Position& B, const std::vector<std::vector<int>>& scores) {
    int x1 = std::min(A.x, B.x);
    int y1 = std::min(A.y, B.y);
    int x2 = std::max(A.x, B.x);
    int y2 = std::max(A.y, B.y);

    std::cout << "A: " << A.x << " : " << A.y << " B: " << B.x << " : " << B.y << std::endl;

    for (int y = y1; y < y2; y++)
    {
        for (int x = x1; x < x2; x++)
        {
            if (scores[x][y] % 2 == 0)
                return false;
        }
    }
    return true;
}

void printScoreboard(const std::vector<std::vector<int>>& scores) {
    for (const auto& row : scores) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << "\n";
    }
}

long long findLargestRectangle2(const std::vector<Position>& positions) {
    long long out = 0;

    auto getArea = [](const Position& A, const Position B) -> long long {
        long long width = std::abs(A.x - B.x) + 1;
        long long height = std::abs(A.y - B.y) + 1;
        return width * height;
    };

    std::vector<std::vector<int>> scores = getScoreboard(positions);
    printScoreboard(scores);

    for (int i = 0; i < positions.size(); i++) {
        for (int j = i + 1; j < positions.size(); j++) {
            if (isValidRectangle(positions[i], positions[j], scores))
            {
                out = std::max(out, getArea(positions[i], positions[j]));
            }
        }
    }
    return out;
}

int main() {
    std::vector<Position> positions = readDataFromFile("../data.txt");
    std::cout << findLargestRectangle(positions) << std::endl;
    std::cout << findLargestRectangle2(positions) << std::endl;
}