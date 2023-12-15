#include <iostream>
#include <string>
#include <vector>
#include <fstream>


std::vector<std::string> cal_doc = {"1abc2", "pqr3stu8vwx", "a1b2c3d4e5f", "treb7uchet"};


int main() {
    std::cout << "Main" << std::endl;

    // Read file
    std::string filepath = "data/data.txt";
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening the file: " << filepath << std::endl;
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();


    // Logic
    std::string digits_str = "1234567890";
    int sum = 0;

    for (const std::string line : lines) {
        std::size_t first_found = line.find_first_of(digits_str);
        std::size_t last_found = line.find_last_of(digits_str);

        std::string number_str = {line[first_found], line[last_found]};
        int number = std::stoi(number_str);

        sum += number;
    }
    std::cout << sum << std::endl;

    return 0;
}