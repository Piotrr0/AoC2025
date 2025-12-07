#include <iostream>
#include <fstream>
#include <unordered_map>

struct FileOutput {
    std::string stringData;
    size_t lineLength;
};

FileOutput readDataFromFile(const std::string& path) {
    FileOutput fo {.stringData = "", .lineLength = 0};
    std::ifstream dataFile(path);

    if (!dataFile.is_open())
        return fo;

    std::string line;
    while (std::getline(dataFile, line)) {
        fo.stringData += line;
    }
    fo.lineLength = line.size();

    dataFile.close();
    return fo;
}

int getBeamSplitCount(std::string& data, size_t lineLength) {
    int out = 0;
    size_t height = data.size() / lineLength;
    for(int c = 0; c < lineLength; c++) {
        if (data[c] == 'S') data[c] = '|';
    }

    for (int r = 1; r < height; r++) {
        for (int c = 0; c < lineLength; c++) {
            size_t idx = r * lineLength + c;
            size_t idxUp = (r - 1) * lineLength + c;

            if (data[idx] == '.' && data[idxUp] == '|') {
                data[idx] = '|';
            }

            if (data[idx] == '^' && data[idxUp] == '|'){
                out++;
                if (c > 0) 
                    data[idx - 1] = '|';
                if (c < lineLength - 1) 
                    data[idx + 1] = '|';
            }
        }
    }

    return out;
}

std::unordered_map<std::string, long long> timelines;
long long getTimelinesSearch(const std::string& grid, size_t height, size_t width, int r, int c) {
    std::string key = std::to_string(r) + ":" + std::to_string(c);
    
    auto it = timelines.find(key);
    if (it != timelines.end()) {
        return it->second;
    }

    if (r == height - 1) {
        timelines[key] = 1;
        return 1;
    }

    long long res = 0;
    size_t idxDown = (r + 1) * width + c;
    if (grid[idxDown] == '.') {
        res = getTimelinesSearch(grid, height, width, r + 1, c);
    }

    if (grid[idxDown] == '^') {
        res = getTimelinesSearch(grid, height, width, r + 1, c - 1) + getTimelinesSearch(grid, height, width, r + 1, c + 1);
    }

    timelines[key] = res;
    return res;
}

long long getTimelines(std::string& data, size_t lineLength) {    
    size_t height = data.size() / lineLength;

    for(int c = 0; c < lineLength; c++) {
        if (data[c] == 'S') {
            return getTimelinesSearch(data, height, lineLength, 0, c);
        }
    }
    return 0;
}

int main()
{
    FileOutput fo = readDataFromFile("../data.txt");
    std::cout << fo.stringData << std::endl;

    //std::cout << getBeamSplitCount(fo.stringData, fo.lineLength) << std::endl;
    std::cout << getTimelines(fo.stringData, fo.lineLength) << std::endl;
    return 0;
}