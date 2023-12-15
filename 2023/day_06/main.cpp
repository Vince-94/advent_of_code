#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define TEST false


std::vector<std::string> read_file(std::string filepath)
{
    std::vector<std::string> lines{};
    std::string line{};

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error in opening the file: " << filepath << std::endl;
        return lines;
    }

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    return lines;
}


bool calculate(int total_time, int hold_time, int target)
{
    int count = 0;
    auto speed = hold_time;
    auto distance = speed*(total_time-hold_time);
    if (distance > target) {
        return true;
    }
    return false;
}


int main()
{
    std::cout << "Day 6" << std::endl;

    // Open file
    std::string filename = "data.txt";
    if (TEST) {
        filename = "test_data.txt";
    }
    std::string filepath = "data/" + filename;

    std::vector<std::string> lines = read_file(filepath);

    // Extract data
    std::map<std::string, std::vector<long long>> data_raw{};

    for (const auto& line : lines)  {
        std::size_t colons_it = line.find(":");
        std::string title = line.substr(0, colons_it);

        std::size_t payload_it = line.find_first_not_of(" ", colons_it+1);
        std::string payload_str = line.substr(payload_it, line.size()-payload_it);

        std::string temp_str{};
        std::vector<long long> payload{};
        auto x = payload_str.end();
        for (auto it = payload_str.begin(); it != payload_str.end(); ++it) {
            char ch = *it;

            if (isdigit(ch)) {
                temp_str.push_back(ch);
            }

            if (it == payload_str.end()-1) {
                auto temp = std::stol(temp_str);
                payload.push_back(temp);
            }

        }
        data_raw[title] = payload;
    }

    // Repack
    int races_num = data_raw["Time"].size();
    std::vector<std::vector<int>> races(races_num);

    for (const auto& pair : data_raw) {
        int i = 0;
        auto vector = pair.second;
        for (const auto& num : vector) {
            races[i].push_back(num);
            i++;
        }
    }

    // Logic
    long long res = 1;
    for (auto race : races) {
        long long count = 0;
        auto total_time = race[1];
        auto target_distance = race[0];
        for (auto hold_time = 0; hold_time < total_time; hold_time++) {
            auto speed = hold_time;
            auto distance = speed*(total_time-hold_time);
            if (distance > target_distance) {
                count++;
            }
        }
        std::cout << res << std::endl;
        res *= count;
    }


    return 0;
}