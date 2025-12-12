#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>

std::vector<std::string> readDataFromFile(const std::string& path) {
    std::vector<std::string> out;
    std::ifstream dataFile(path);

    if (!dataFile.is_open())
        return {};

    std::string line;
    while (std::getline(dataFile, line)) {
        out.emplace_back(line);
    }

    dataFile.close();
    return out;
}

struct Region {
    int size;
    std::vector<int> presents;
};

Region parseRegion(std::string regionLine)
{
    Region reg;
    std::stringstream ss(regionLine);
    
    int height, width;
    char delimiter;

    ss >> height >> delimiter >> width >> delimiter;

    reg.size = height * width;

    int present;
    while (ss >> present) {
        reg.presents.emplace_back(present);
    }

    return reg;
}

struct Present {
    int name;
    int size; 
};

Present parsePresent(const std::vector<std::string>& data, int& i)
{
    Present p {.name = 0, .size = 0};

    while (i < data.size())
    {
        std::string line = data[i];
        if (line.empty() || std::isdigit(line[0])) {
            break; 
        }

        for (char c : line) {
            if (c == '#') {
                p.size++;
            }
        }

        i++;
    }

    size_t colonPos = data[i].find(':');
    if (colonPos != std::string::npos) {
        p.name = std::stoi(data[i].substr(0, colonPos));
    }

    return p;
}


int main() {
    std::vector<std::string> data = readDataFromFile("../data.txt");
    std::reverse(data.begin(), data.end());

    int i = 0;

    std::vector<Region> regions;
    while (i < data.size() && data[i].size() > 1)
    {
        Region r = parseRegion(data[i]);
        regions.emplace_back(r);
        i++;
    }

    i++;

    std::unordered_map<int, int> map;
    while (i < data.size())
    {
        Present p = parsePresent(data, i);
        std::cout << p.name << ":" << p.size << std::endl;
        map[p.name] = p.size;
        i+=2;
    }

    int out = 0;
    for (const auto& reg : regions) {
        int total = 0;
        for (int i = 0; i < reg.presents.size(); i++)
        {
            total += map[i] * reg.presents[i];
        }

        if (reg.size > total)
            out++;
    }
    std::cout << out;
}