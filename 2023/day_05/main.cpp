#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <tuple>
#include <stdexcept>  // For std::runtime_error


#define TEST false


/**
 * @brief 
 * 
 * @param filepath 
 * @return std::vector<std::string> 
 */
std::vector<std::string> read_file(std::string filepath)
{
    std::vector<std::string> lines{};
    std::string line{};

    std::ifstream file(filepath);

    // Check if file has been correctly opened
    if (!file.is_open()) {
        std::cerr << "Error in opening the file: " << filepath << std::endl;
        return lines;
    }

    // Read file line by line
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    // Close file
    file.close();

    return lines;
}


/**
 * @brief 
 * 
 * @param line 
 * @return std::vector<int> 
 */
std::vector<int> extract_val_from_str(std::string line)
{
    std::vector<int> val{};

    std::size_t line_idx = 0;
    std::size_t space_idx = line.find(" ");
    while (space_idx != std::string::npos) {
        auto val_str = line.substr(line_idx, space_idx-line_idx);
        auto temp = std::stoll(val_str);
        val.push_back(temp);
        line_idx = space_idx + 1;
        space_idx = line.find(" ", line_idx);
    }
    auto val_str = line.substr(line_idx, space_idx-line_idx);
    val.push_back(std::stoi(val_str));

    return val;
}



std::tuple<std::vector<int>, std::vector<int>> extract_mapping_info(std::vector<int> mapping)
{
    // std::vector<std::vector<int>> remap{};
    std::tuple<std::vector<int>, std::vector<int>> remap{};

    int destination = mapping[0];
    int source = mapping[1];
    int length = mapping[2];

    std::vector<int> source_vec{};
    std::vector<int> destination_vec{};

    for (int i=0; i < length; i++) {
        source_vec.push_back(source + i);
        destination_vec.push_back(destination + i);
    }

    remap = std::make_tuple(source_vec, destination_vec);

    // remap.push_back(source_vec);
    // remap.push_back(destination_vec);

    return remap;
}


std::map<int, int> generate_partial_map(std::vector<std::tuple<std::vector<int>, std::vector<int>>> function_map)
{
    std::map<int, int> map{};

    for (int i = 0; i < function_map.size(); i++) {

        auto source_v = std::get<0>(function_map[i]);
        auto destination_v = std::get<1>(function_map[i]);

        if (source_v.size() != destination_v.size()) {
            throw std::runtime_error("Source list does not match destination list");
        }

        for (int j = 0; j < source_v.size(); j++) {
            map.emplace(source_v[j], destination_v[j]);
        }
    }
    return map;
}


// TODO
std::map<int, int> generate_full_map(std::vector<std::tuple<std::vector<int>, std::vector<int>>> function_map)
{
    std::map<int, int> partial_map = generate_partial_map(function_map);

    // Create a full list
    int j = 0;
    for (int i = 0; i < 10000000000; i++) {
        if (partial_map.count(i) > 0) {
            continue;
        }

        for (int j = 0; j < 10000000000; j++) {
            if (partial_map.find(j) == partial_map.end()) {
                partial_map.emplace(i, j);
                break;
            }
        }
    }

    return partial_map;
}


int main()
{
    std::cout << "Day 5" << std::endl;

    //! Read file
    std::string filename = "data.txt";
    if (TEST) {
        filename = "test_data.txt";
    }
    std::string filepath = "data/" + filename;
    std::vector<std::string> lines = read_file(filepath);

    if (lines.empty()) {
        std::cerr << "File is empty" << std::endl;
        return 0;
    }

    //! Parsing data
    std::vector<int> seeds{};
    bool in_map = false;

    std::map<std::string, std::vector<std::vector<int>>> mapping_functions{};
    std::string map_key{};
    std::vector<std::vector<int>> map_values{};

    for (auto line : lines) {

        // Get seeds
        std::size_t seeds_idx = line.find("seeds");
        if (seeds_idx != std::string::npos) {
            std::size_t colons_idx = line.find(":") + 2;
            auto sub_str = line.substr(colons_idx, line.size()-colons_idx);
            seeds = extract_val_from_str(sub_str);
            continue;
        }

        // Get mapping_functions
        std::size_t map_idx = line.find("map");
        if (map_idx != std::string::npos) {
            in_map = true;
            map_values.clear();
            map_key = line;
        }

        if (in_map && !line.empty() && map_idx == std::string::npos) {
            map_values.push_back(extract_val_from_str(line));
        }

        if (in_map && (line.empty() || line == lines.back())) {
            // Create map_values
            mapping_functions.emplace(map_key, std::move(map_values));

            // Reset conditions
            in_map = false;
        }
    }


    //! Logic
    std::map<std::string, std::map<int, int>> maps{};
    for (auto& pair : mapping_functions) {
        std::string title = pair.first;
        std::vector<std::vector<int>> map_v = pair.second;
        std::vector<std::tuple<std::vector<int>, std::vector<int>>> new_map{};

        for (std::vector<int> map : map_v) {
            new_map.push_back(extract_mapping_info(map));
        }

        auto map = generate_full_map(new_map);
        maps.emplace(title, map);
    }

    auto seed_to_soil_it = maps.find("seed-to-soil map:");
    auto soil_to_fertilizer_it = maps.find("soil-to-fertilizer map:");
    auto fertilizer_to_water_it = maps.find("fertilizer-to-water map:");
    auto water_to_light_it = maps.find("water-to-light map:");
    auto light_to_temperature_it = maps.find("light-to-temperature map:");
    auto temperature_to_humidity_it = maps.find("temperature-to-humidity map:");
    auto humidity_to_location_it = maps.find("humidity-to-location map:");

    int min = 100;
    for (auto seed : seeds) {
        auto soil = (seed_to_soil_it->second).find(seed)->second;
        auto fertilizer = (soil_to_fertilizer_it->second).find(soil)->second;
        auto water = (fertilizer_to_water_it->second).find(fertilizer)->second;
        auto light = (water_to_light_it->second).find(water)->second;
        auto temperature = (light_to_temperature_it->second).find(light)->second;
        auto humidity = (temperature_to_humidity_it->second).find(temperature)->second;
        auto location = (humidity_to_location_it->second).find(humidity)->second;
        if (location < min) {
            min = location;
        }
    }

    std::cout << "Min = " << min << std::endl;



    return 0;
}