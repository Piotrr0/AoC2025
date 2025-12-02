#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::string readDataFromFile(const std::string& path) {
    std::string data;
    std::ifstream dataFile(path);

    if (!dataFile.is_open()) {
        return "";
    }

    std::string line;
    std::getline(dataFile, line);
    data += line;

    dataFile.close();
    return data;
}

struct Range {
    long long start = -1;
    long long end = -1;
};

long long parseNumber(const std::string& dataString, size_t& idx) {
    std::string numberString = "";
    while (idx < dataString.size() && std::isdigit(dataString[idx]))
    {
        numberString += dataString[idx];
        idx++;
    }

    return std::stoll(numberString);
}

std::vector<Range> parseDataString(const std::string& dataString) {
    std::vector<Range> out;

    size_t i = 0;
    while (i < dataString.size()) {
        long long start = parseNumber(dataString, i);
        i++;
        long long end =  parseNumber(dataString, i);

        if (dataString[i] == ',') {
            i++;
        }
        Range range {.start = start, .end = end};
        out.emplace_back(range);
    }
    
    return out;
}

int getDigitsCount(long long num)
{
    int digits = 0;
    while (num > 0) {
        digits++;
        num /= 10;
    }

    return digits;
}

struct NumberHalves {
    long long left;
    long long right;
};

NumberHalves splitNumberInHalf(long long num, int digits) {
    int divisor = 1;
    for (int i = 0; i < digits/2; i++) {
        divisor *= 10;
    }

    int right = num % divisor;
    int left  = num / divisor;

    return NumberHalves {.left = left, .right = right};
}

long long sumInvalidIds(const std::vector<Range>& ranges) {
    long long out = 0;

    for (const auto& range : ranges) {
        for (long long num = range.start; num <= range.end; num++)
        {
            int digits = getDigitsCount(num);

            if (digits % 2 == 0)
            {
                NumberHalves halfs = splitNumberInHalf(num, digits);
                if (halfs.left == halfs.right)
                    out += num;
            }
        }
    }
    return out;
}

std::vector<int> getDividers(int num) {
    std::vector<int> out;
    for(int i = 1; i <= num; i++) {
        if (num % i == 0)
            out.emplace_back(i);
    }
    return out;
}

std::vector<int> splitNumberInParts(long long num, int digits, int parts) {
    std::vector<int> result;
    result.reserve(parts);

    int groupSize = digits / parts;
    int divisor = 1;

    for (int i = 0; i < groupSize; i++)
        divisor *= 10;

    for (int i = 0; i < parts; i++) {
        int chunk = num % divisor;
        result.emplace_back(chunk);

        num /= divisor;
    }

    return result;
}

bool allEqual(const std::vector<int>& v) {
    if (v.empty()) 
        return false;

    for (int i = 1; i < v.size(); i++) {
        if (v[0] != v[i])
            return false;
    }
    return true;
}

long long sumInvalidIds2(const std::vector<Range>& ranges) {
    long long out = 0;

    for (const auto& range : ranges) {
        for (long long num = range.start; num <= range.end; num++) 
        {
            int digits = getDigitsCount(num);
            std::vector<int> divs = getDividers(digits);

            for (int div : divs)
            {
                if (div == 1)
                    continue;

                std::vector<int> split = splitNumberInParts(num, digits, div);
                if (allEqual(split)) {
                    out += num;
                    break;
                } 
            }
        }
    }
    return out;
}

int main() {
    std::string data = readDataFromFile("../data.txt");
    std::vector<Range> ranges = parseDataString(data);

    for (const auto& range : ranges) {
        std::cout << range.start << " " << range.end << std::endl;
    }

    std::cout << std::endl << sumInvalidIds(ranges);
    std::cout << std::endl << sumInvalidIds2(ranges);
}