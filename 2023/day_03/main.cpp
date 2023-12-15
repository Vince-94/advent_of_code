#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <tuple>

#define TEST true


std::string digit = "0123456789";
std::string point = ".";
auto str_to_check = digit + point;


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

    // Read file
    std::string filename = "data.txt";
    if (TEST) {
        filename = "test_data.txt";
    }
    std::string filepath = "data/" + filename;
    std::vector<std::string> lines = read_file(filepath);


    // Logic
    int row_min = 0;
    int col_min = 0;
    int row_max = lines.size();
    int col_max = lines[0].size() - 1;

    int sum_num = 0;
    for (int i = 0; i < row_max; i++) {
        std::cout << lines[i] << std::endl;

        std::size_t init_num_idx = 0;
        std::size_t end_num_idx = 0;

        // first occurance
        init_num_idx = lines[i].find_first_of(digit, init_num_idx);
        end_num_idx = lines[i].find_first_not_of(digit, init_num_idx);

        // further occurances
        while (end_num_idx != std::string::npos) {
            // Check symbols
            bool status = false;
            for (int k = i-1; k <= i+1; k++) {
                if (k < row_min || k > row_max) continue;

                for (int j = int(init_num_idx)-1; j <= int(end_num_idx)+1; j++) {
                    if (j < col_min || j > col_max) continue;
                    if (i == k && j >= init_num_idx && j < end_num_idx) continue;

                    auto ch = lines[k][j];
                    if (str_to_check.find(ch) == std::string::npos) {
                        status = true;
                        std::string num_str = lines[i].substr(init_num_idx, end_num_idx-init_num_idx);
                        int num = std::stoi(num_str);
                        sum_num += num;
                        break;
                    }
                }
                if (status == true) break;
            }

            // Update index
            init_num_idx = end_num_idx + 1;
            init_num_idx = lines[i].find_first_of(digit, init_num_idx);
            end_num_idx = lines[i].find_first_not_of(digit, init_num_idx);
        }

    }

    std::cout << "sum_num = " << sum_num << std::endl;  // 540945 < x < 544733
    return 0;
}