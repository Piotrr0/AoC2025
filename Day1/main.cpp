#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::string readFromFile(const std::string& path) {
    std::string data;
    std::ifstream dataFile(path);

    if (!dataFile.is_open()) {
        return "";
    }

    std::string line;
    while (std::getline(dataFile, line)) {
        data += line;
    }

    dataFile.close();
    return data;
}

struct Rotation {
    enum class Dir {
        Left,
        Right,
        None
    };

    Dir dir;
    int steps;
};

Rotation::Dir parseDirection(char ch) {

    if (ch == 'L')
    {
        return Rotation::Dir::Left;
    }
    else if (ch == 'R')
    {
        return Rotation::Dir::Right;
    }

    return Rotation::Dir::None;
}

int parseStep(const std::string& data, size_t& startingIndex)
{
    std::string step;
    while (startingIndex < data.size() && std::isdigit(data[startingIndex])) {
        step += data[startingIndex];
        startingIndex++;
    }

    return std::stoi(step);
}

std::vector<Rotation> parseRotationData(const std::string& data) {
    std::vector<Rotation> rotations;
    size_t i = 0;

    while (i < data.size()) {
    
        Rotation::Dir dir = parseDirection(data[i]);
        i++;

        int step = parseStep(data, i);

        Rotation rot {.dir = dir, .steps =step };
        rotations.emplace_back(rot);
    }

    return rotations;
}

struct DialOutput {
    int start = 0;
    int clicks = 0;
};

DialOutput calculateDialRotation(const std::vector<Rotation>& rotations, int start = 50, int limit = 100) {
    DialOutput output;
    output.start = start;

    for (const auto& rot : rotations) {
        if (rot.dir == Rotation::Dir::Left){
            output.start -= rot.steps;
            output.start = ((output.start %= limit) < 0) ? output.start+limit : output.start;
        }
        else if (rot.dir == Rotation::Dir::Right) {
            output.start = (output.start + rot.steps) % limit;
        }

        std::cout << output.start << std::endl;

        if (output.start == 0) {
            output.clicks++;
        }
    }

    return output;
}

DialOutput calculateDialRotationWithPassword(const std::vector<Rotation>& rotations, int start = 50, int peak = 100) {
    DialOutput output;
    output.start = start;

    for (const auto& rot : rotations) {
        int stepsRemaining = rot.steps;        
        if (rot.dir == Rotation::Dir::Left) {
            int stepsTo0 = output.start == 0 ? peak : output.start;

            if (stepsRemaining >= stepsTo0) {
                output.clicks++;

                stepsRemaining -= stepsTo0;
                output.clicks += stepsRemaining / peak;
            }
            
            output.start = ((output.start - rot.steps) % peak + peak) % peak;
        }
        else if (rot.dir == Rotation::Dir::Right) {
            int stepsTo0 = output.start == 0 ? peak : (peak - output.start) % peak;

            if (stepsRemaining >= stepsTo0) {
                output.clicks++;

                stepsRemaining -= stepsTo0;
                output.clicks += stepsRemaining / peak;
            }

            output.start = (output.start + rot.steps) % peak;
        }

        std::cout << output.start << std::endl;
    }

    return output;
}

int main() {
    std::string data = readFromFile("../data.txt");
    std::vector<Rotation> rots = parseRotationData(data);

    const DialOutput output = calculateDialRotation(rots);
    const DialOutput outputWithPassword = calculateDialRotationWithPassword(rots);

    std::cout << std::endl << output.clicks;
    std::cout << std::endl << outputWithPassword.clicks;

    return 0;
}