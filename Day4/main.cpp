#include <iostream>
#include <fstream>
#include <vector>

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

int countAccessibleRolls(const std::string& dataString, size_t lineLength) {
    if (lineLength == 0) 
        return 0;

    int out = 0;
    int dx[] = {1, 1, 1, 0, 0, -1, -1, -1};
    int dy[] = {1, 0, -1, 1, -1, 1, 0, -1};
    
    int height = dataString.size() / lineLength;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineLength; x++)
        {
            int idx = y * lineLength + x;
            char current = dataString[idx];
            if (current != '@')
                continue;

            int neighbors = 0;
            for (int i = 0; i < 8; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (nx < 0 || ny < 0 || nx >= lineLength || ny >= height)
                    continue;

                int nidx = ny * lineLength + nx;
                if (dataString[nidx] == '@')
                    neighbors++;
            }

            if (neighbors < 4)
                out++;
        }
    }

    return out; 
}

int countAccessibleRolls2(std::string& dataString, size_t lineLength) {
    if (lineLength == 0) 
        return 0;

    int out = 0;
    int dx[] = {1, 1, 1, 0, 0, -1, -1, -1};
    int dy[] = {1, 0, -1, 1, -1, 1, 0, -1};

    int height = dataString.size() / lineLength;
    int accessibleCount = 0;

    do {
        std::vector<int> accesiblePositions;
        accessibleCount = 0;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < lineLength; x++)
            {
                int idx = y * lineLength + x;
                char current = dataString[idx];
                if (current != '@')
                    continue;
    
                int neighbors = 0;
                for (int i = 0; i < 8; i++) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    if (nx < 0 || ny < 0 || nx >= lineLength || ny >= height)
                        continue;
    
                    int nidx = ny * lineLength + nx;
                    if (dataString[nidx] == '@')
                        neighbors++;
                }

                if (neighbors < 4) {
                    out++;
                    accessibleCount++;
                    accesiblePositions.push_back(y * lineLength + x);
                }
            }
    
            for (const auto& idx : accesiblePositions)
            {
                dataString[idx] = '.';
            }
        }
    } while (accessibleCount > 0);

    return out; 
}


int main()
{
    FileOutput fo = readDataFromFile("../data.txt");
    std::cout << countAccessibleRolls(fo.stringData, fo.lineLength) << std::endl;
    std::cout << countAccessibleRolls2(fo.stringData, fo.lineLength) << std::endl;
}