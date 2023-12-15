#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <tuple>

#define TEST false


struct Cubes
{
    int cube_red = 0;
    int cube_green = 0;
    int cube_blue = 0;
};


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



// "1 green" -> {"green": 1}
std::tuple<std::string, int> get_cube_map(std::string cube_str)
{
    // std::cout << "     - cube: " << cube_str << std::endl;

    auto pair_idx = 0;
    std::size_t space_idx = cube_str.find(" ", pair_idx);

    std::string cube_val = cube_str.substr(pair_idx, space_idx-pair_idx);
    std::string cube_color = cube_str.substr(space_idx+1, cube_str.size()-space_idx+1);

    int value = std::stoi(cube_val);

    std::tuple<std::string, int> cube = {cube_color, value};

    return cube;
}


// "1 green, 2 red, 3 blue" -> {"green": 1, "red": 2, "blue": 3}
std::map<std::string, int> get_set_value(std::string set)
{
    // std::cout << "   - set: " << set << std::endl;

    auto set_idx = 0;
    std::map<std::string, int> cubes_set{};

    // first occurrence
    std::size_t comma_idx = set.find(",", set_idx);

    // further occurrences
    while (comma_idx != std::string::npos) {
        auto cube = get_cube_map(set.substr(set_idx, comma_idx-set_idx));
        cubes_set[std::get<0>(cube)] = std::get<1>(cube);

        set_idx = comma_idx + 2;
        comma_idx = set.find(",", set_idx);
    }

    // last occurrence
    auto cube = get_cube_map(set.substr(set_idx, set.size()-set_idx));
    cubes_set[std::get<0>(cube)] = std::get<1>(cube);

    return cubes_set;
}


// "3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green" -> [{"red": 5, "blue": 3}, {"green": 2, "red": 1, "blue": 6}, {"green": 2}]
std::vector<std::map<std::string, int>> get_line_value(std::string line)
{
    // std::cout << " - line: " << line << std::endl;

    auto line_idx = 0;
    std::vector<std::map<std::string, int>> line_cubes_vector{};

    // First occurence
    std::size_t semicolon_idx = line.find(";", line_idx);

    // further occurrences
    while (semicolon_idx != std::string::npos) {
        auto set_cubes = get_set_value(line.substr(line_idx, semicolon_idx-line_idx));
        line_cubes_vector.push_back(set_cubes);

        line_idx = semicolon_idx + 2;
        semicolon_idx = line.find(";", line_idx);
    }

    // last occurrence
    auto set_cubes = get_set_value(line.substr(line_idx, line.size()-line_idx));
    line_cubes_vector.push_back(set_cubes);

    return line_cubes_vector;
}


int calculate(std::string line, std::map<std::string, int> solution) {
    std::cout << line << std::endl;

    // Find id: "Game id: i1 str1, i2 str2; i3 str3, i4 str4, i5 str5; i6 str6"
    std::size_t colons_idx = line.find(":");
    int id_length = colons_idx - 5;
    std::string id_str = line.substr(5, id_length);
    int id = std::stoi(id_str);  //? Index of line

    auto line_idx = colons_idx + 2;
    auto line_cubes_vector = get_line_value(line.substr(line_idx, line.size()-line_idx));

    // Check if the solution is infeasible
    // for (auto& line_cubes : line_cubes_vector) {
    //     if (line_cubes["red"] > solution["red"] || line_cubes["green"] > solution["green"] || line_cubes["blue"] > solution["blue"]) {
    //         return 0;
    //     }
    // }

    // Find the max cubes
    std::map<std::string, int> max_cube;
    for (auto& line_cubes : line_cubes_vector) {
        for (const auto& pair : line_cubes) {
            max_cube[pair.first] = std::max(max_cube[pair.first], pair.second);
        }
    }

    int sum = max_cube["red"] * max_cube["green"] * max_cube["blue"];

    return sum;
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

    // for (std::string line : lines) {
    //     std::cout << line << std::endl;
    // }


    //! Logic
    std::map<std::string, int> solution{};
    solution["red"] = 12;
    solution["green"] = 13;
    solution["blue"] = 14;

    int idx = 0;

    for (std::string line : lines) {
        idx += calculate(line, solution);
        // std::cout << " - idx: " << idx << std::endl;
    }

    std::cout << "Solution: " << idx << std::endl;  // 1728 < sol < 4000


    return 0;
}