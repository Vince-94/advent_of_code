#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>


std::vector<std::string> read_file(std::string filepath) {
    std::ifstream file(filepath);
    std::vector<std::string> lines{};
    std::string line{};

    if (!file.is_open()) {
        std::cerr << "Error opening the file: " << filepath << std::endl;
        return lines;
    }

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    return lines;
}



int main() {
    std::cout << "Main" << std::endl;

    // Test
    // std::vector<std::string> lines = {"two1nine", "eightwothree", "abcone2threexyz", "xtwone3four", "4nineeightseven2", "zoneight234", "7pqrstsixteen"};

    // Read file
    std::string filepath = "data/data.txt";
    std::vector<std::string> lines = read_file(filepath);



    //! Logic
    std::string integer_digits = "0123456789";
    std::vector<std::string> letter_digits = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    int sum = 0;

    for (std::string line : lines) {
        std::cout << "Line: " << line;

        // Find all the occurencies
        std::map<int, int> occurencies;  // - map {key = found, value = idx}
        for (int idx = 0; idx < letter_digits.size(); idx++) {
            std::size_t found = line.find(letter_digits[idx]);

            while (found!=std::string::npos) {
                occurencies[found] = idx;
                found = line.find(letter_digits[idx], line[found] + letter_digits[idx].size()+1);
            }
        }

        // Replacing the first and the last element if found
        if (!occurencies.empty()) {
            auto first_str = occurencies.begin();
            auto last_str = std::prev(occurencies.end());

            if (first_str == last_str) {
                line.replace(first_str->first, letter_digits[first_str->second].size(), 1, integer_digits[first_str->second]);
            } else if (first_str->first + letter_digits[first_str->second].size() > last_str->first) {
                std::cout << "Overlapping" << std:: endl;
                line.replace(first_str->first, letter_digits[first_str->second].size(), 1, integer_digits[first_str->second]);
            } else {
                line.replace(last_str->first, letter_digits[last_str->second].size(), 1, integer_digits[last_str->second]);
                line.replace(first_str->first, letter_digits[first_str->second].size(), 1, integer_digits[first_str->second]);
            }
        }
        std::cout << " -> " << line << std:: endl;

        // Find the edges numbers
        std::size_t first_found = line.find_first_of(integer_digits);
        std::size_t last_found = line.find_last_of(integer_digits);

        int number = 0;
        if (first_found == last_found) {
            std::string found = {line[first_found]};
            number = std::stoi(found);
        } else {
            std::string number_str = {line[first_found], line[last_found]};
            number = std::stoi(number_str);
        }

        sum += number;
    }
    std::cout << sum << std::endl;

    return 0;
}