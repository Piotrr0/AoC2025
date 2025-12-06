#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define PROBLEM_COLS 1000
#define NUMBER_ROWS 4

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
        line += ";";
        fo.stringData += line;
    }
    fo.lineLength = line.size();

    dataFile.close();
    return fo;
}

struct Problem {
    std::vector<long long> numbers;
    char op;
};

std::vector<Problem> parseDataString(const std::string& data) {
    std::vector<Problem> problems(PROBLEM_COLS);

    int numberCount = 0;
    int opCount = 0;
    std::string value = "";

    for (int i = 0; i < data.size(); i++) {
        char current = data[i];
        if (std::isdigit(current))
        {
            value += current;
        }
        else if (current == '+' || current == '*') {
            problems[opCount++].op = current;
        }
        else if (!value.empty())
        {
            int idx = numberCount++ % PROBLEM_COLS;
            problems[idx].numbers.emplace_back(std::stoll(value));
            value.clear();
        }
    }
    return problems;
}

std::vector<Problem> parseDataString2(const std::string& data, size_t lineLength) {
    std::vector<Problem> problems(PROBLEM_COLS);

    int problemCount = 0;
    for (int col = lineLength - 1; col >= 0; col--) {
        std::string value = "";
        for (int row = 0; row < NUMBER_ROWS; row++) {
            size_t idx = row * lineLength + col;
            if (idx >= data.size())
                break;

            char current = data[idx];
            if (std::isdigit(current)) {
                value += current;
            }
        }

        if (!value.empty())
            problems[problemCount].numbers.emplace_back(std::stoll(value));

        size_t opIdx = NUMBER_ROWS * lineLength + col;
        if (opIdx >= data.size())
            break;

        char opChar = data[opIdx];
        if (opChar == '+' || opChar == '*')
            problems[problemCount++].op = opChar;
    }

    return problems;
}

long long solveProblem(const Problem& problem) {
    long long out = (problem.op == '*') ? 1 : 0;

    for(const auto num : problem.numbers) {
        if (problem.op == '+')
        {
            out += num;
        }
        else if (problem.op == '*') {
            out *= num; 
        }
    }
    return out;
}

long long solveProblemSheet(const std::vector<Problem>& problems) {
    long long sum = 0;
    for (const auto& problem : problems) {
        sum += solveProblem(problem);
    }
    return sum;
}

int main()
{
    FileOutput fo = readDataFromFile("../data.txt");
    std::cout << fo.stringData << std::endl;

    std::vector<Problem> problems = parseDataString(fo.stringData);
    std::cout << solveProblemSheet(problems) << std::endl;

    std::vector<Problem> problems2 = parseDataString2(fo.stringData, fo.lineLength);
    std::cout << solveProblemSheet(problems2);

    return 0;
}