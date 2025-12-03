#include <iostream>
#include <fstream>
#include <string_view>
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

long long findLargestJoltage(std::string_view number) {
    int maxJoltage = 0;
    int maxOnRight = -1;
    for (int j = number.size() - 1; j >= 0; j--)
    {
        int currentDigit = number[j] - '0';
        if (maxOnRight >= 0) {
            int joltage = currentDigit * 10 + maxOnRight;
            maxJoltage = std::max(maxJoltage, joltage);
        }
        maxOnRight = std::max(maxOnRight, currentDigit);
    }
    return maxJoltage;
}

int calculateMaxOutputJoltage(const FileOutput& fo) {
    int out = 0;

    size_t size = fo.stringData.size();
    for (size_t i = 0; i < size; i += fo.lineLength) {
        size_t chunkSize = std::min(fo.lineLength, size - i);
        std::string_view sv{fo.stringData.data() + i, chunkSize};
        
        out += findLargestJoltage(sv);
    }

    return out;
}

long long pow10(int exp) {
    long long res = 1;
    for (int i = 0; i < exp; ++i) {
        res *= 10;
    }

    return res;
}

long long findLargestJoltage(std::string_view number, int digits, std::unordered_map<std::string, long long>& memo)
{
    if (digits == 0)
        return 0;

    std::string stringNumber = std::string(number);
    if (number.size() == digits)
        return stoll(stringNumber);

    std::string key = stringNumber + ":" + std::to_string(digits);
    auto it = memo.find(key);
    if (memo.find(key) != memo.end())
        return it->second;

    long long digitValue = number[0] - '0';
    long long power = pow10(digits - 1);

    long long a = digitValue * power + findLargestJoltage(number.substr(1), digits - 1, memo);
    long long b = findLargestJoltage(number.substr(1), digits, memo);
    long long result = std::max(a, b);

    memo[key] = result;
    return result;
}

long long calculateMaxOutputJoltage2(const FileOutput& fo) {
    long long out = 0;

    size_t size = fo.stringData.size();
    for (size_t i = 0; i < size; i += fo.lineLength) {
        size_t chunkSize = std::min(fo.lineLength, size - i);
        std::string_view sv{fo.stringData.data() + i, chunkSize};

        std::unordered_map<std::string, long long> memo;
        out += findLargestJoltage(sv, 12, memo);
    }

    return out;
}

int main()
{
    FileOutput fo = readDataFromFile("../data.txt");
    std::cout << calculateMaxOutputJoltage(fo) << std::endl;
    std::cout << calculateMaxOutputJoltage2(fo) << std::endl;
}