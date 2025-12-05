#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

struct FileOutput {
    std::string ranges;
    std::string ids;
};

FileOutput readFromFile(const std::string& path) {
    FileOutput data;
    std::ifstream dataFile(path);

    if (!dataFile.is_open()) {
        return FileOutput {.ranges = "", .ids = ""};
    }

    std::string line;
    while (std::getline(dataFile, line)) {
        if (line.size() == 0)
            break;

        data.ranges += line + ";";
    }

    while (std::getline(dataFile, line)) {
        if (line.size() == 0)
            break;
        
        data.ids += line + ";";
    }

    dataFile.close();
    return data;
}

struct IngredientsList {
    std::vector<std::pair<long long, long long>> ranges;
    std::vector<long long> ids;
};

IngredientsList parseDataString(const std::string& ranges, const std::string& ids) {
    IngredientsList out;

    for (int i = 0; i < ranges.size(); i++)
    {
        std::string start;
        while (std::isdigit(ranges[i]) && i < ranges.size())
        {
            start += ranges[i];
            i++;
        }
        
        if (ranges[i] != '-')
            break;

        i++;

        std::string end;
        while (std::isdigit(ranges[i]))
        {
            end += ranges[i];
            i++;
        }

        if (ranges[i] != ';')
            break;

        out.ranges.emplace_back(std::pair<long long, long long>{std::stoll(start), std::stoll(end)});
    }

    for (int i = 0; i < ids.size(); i++)
    {
        std::string value;
        while (std::isdigit(ids[i]) && i < ids.size())
        {
            value += ids[i];
            i++;
        }

        if (ids[i] != ';')
            break;

        out.ids.emplace_back(std::stoll(value));
    }
    
    return out;
}

int countFreshIngridents(const IngredientsList& list) {
    int out = 0;
    for (const auto id : list.ids) {
        for (const auto& pair : list.ranges) {
            if (id >= pair.first && id <= pair.second) {
                out++;
                break;
            }
        }
    }
    return out;
}

long long countFreshIngridents2(const IngredientsList& list) {
    std::vector<std::pair<long long, long long>> sorted = list.ranges;
    std::sort(sorted.begin(), sorted.end());

    long long out = 0;

    long long currentStart = sorted[0].first;
    long long currentEnd = sorted[0].second;

    for (int i = 1; i < sorted.size(); i++) {
        if (sorted[i].first <= currentEnd + 1) 
        {
            currentEnd = std::max(currentEnd, sorted[i].second);
        }
        else {
            out += currentEnd - currentStart + 1;
            currentStart = sorted[i].first;
            currentEnd = sorted[i].second;
        }
    }

    out += currentEnd - currentStart + 1;
    return out;
}

int main() {

    FileOutput data = readFromFile("../data.txt");
    std::cout << data.ranges << std::endl;
    std::cout << data.ids << std::endl;

    IngredientsList list = parseDataString(data.ranges, data.ids);
    std::cout << countFreshIngridents(list) << std::endl;
    std::cout << countFreshIngridents2(list) << std::endl;

    /*
    for (const auto& pair : list.ranges)
    {
        std::cout << pair.first << " " << pair.second << std::endl;
    }

    for (const int id : list.ids)
    {
        std::cout << id << std::endl;
    }
    */

}