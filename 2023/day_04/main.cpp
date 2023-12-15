#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


#define TEST false


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


int check_win(std::vector<int> win_num, std::vector<int> my_num)
{
    int points = 0;

    for (auto num : my_num) {
        auto found_idx = std::find(win_num.begin(), win_num.end(), num);
        if (found_idx != win_num.end()) {
            if (points == 0) {
                points = 1;
            } else {
                points = points*2;
            }
        }
    }
    return points;
}


std::vector<int> extract_numbers(std::string num_str)
{
    std::vector<int> nums_vec{};
    std::string temp_str{};

    auto x = num_str.end()-1;
    for (auto it = num_str.begin(); it != num_str.end(); ++it) {
        char ch = *it;

        if (std::isdigit(ch)) {
            temp_str.push_back(ch);
        }

        if (!temp_str.empty() && (ch == ' ' || it == num_str.end()-1)) {
            nums_vec.push_back(std::stoi(temp_str));
            temp_str.clear();
        }
    }

    return nums_vec;
}


int compute(std::string line)
{
    std::cout << "Line: " << line << std::endl;
    int points{};

    std::size_t bar_idx = line.find("|");

    std::string win_num_str = line.substr(0, bar_idx-1);
    std::vector<int> win_num = extract_numbers(win_num_str);

    std::string my_num_str = line.substr(bar_idx+2, line.size());
    std::vector<int> my_num = extract_numbers(my_num_str);

    points = check_win(win_num, my_num);

    std::cout << "-> points = " << points << std::endl;
    return points;
}


int main() {
    std::cout << "Day 4" << std::endl;

    // Read file
    std::string filename = "data.txt";
    if (TEST) {
        filename = "test_data.txt";
    }
    std::string filepath = "data/" + filename;
    std::vector<std::string> lines = read_file(filepath);

    if (lines.empty()) {
        std::cerr << "Empty file" << std::endl;
        return 0;
    }

    // Logic
    int tot_points = 0;
    for (std::string& line : lines) {
        std::size_t idx = line.find(":") + 2;
        line = line.substr(idx, line.size()-idx);
        int points = compute(line);
        tot_points += points;
    }
    std::cout << "Total points = " << tot_points << std::endl;

    return 0;
}
