#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>

#define TEST true


struct Hand
{
    std::vector<int> cards{};
    int bet{};
};



class OrderingRules
{
public:
    OrderingRules() {};

    int first_rule(std::unordered_map<int, int>& hand_map)
    {
        int category{};
        int three_card = 0;
        int two_card = 0;
        for (auto it = hand_map.begin(); it != hand_map.end(); ++it) {

            if (it->second == 5) {  // five_of_a_kind
                category = 6;
                break;
            } else if (it->second == 4) {  // four_of_a_kind
                category = 5;
                break;
            }

            if (it->second == 3) {
                three_card++;
            } else if (it->second == 2) {
                two_card++;
            }

            if (three_card == 1 && two_card == 1) {  // full_house
                category = 4;
                break;
            } else if (three_card == 1 && two_card == 0) {  // three_of_a_kind
                category = 3;
            } else if (two_card == 2) {  // two_pair
                category = 2;
                break;
            } else if (two_card == 1) {  // one_pair
                category = 1;
            }

        }

        return category;
    }

    void second_rule()
    {
        ;
    }

};


/**
 * @brief CamelGame
 * 
 */
class CamelGame
{
public:
    /**
     * @brief Construct a new Camel Game object
     * 
     * @param hands 
     */
    CamelGame(std::vector<Hand> hands) : _hands{hands} {};

    void play()
    {
        // First ordering rule
        for (Hand& hand : _hands) {
            std::unordered_map<int, int> hand_map = generate_hand_map(hand.cards);
            int cat = _ordering->first_rule(hand_map);
            _hand_map[cat].push_back(hand);
        }

        //TODO Second ordering rule
        std::cout << "Game finished" << std::endl;
    }

private:
    /// @brief 
    std::vector<Hand> _hands;
    std::shared_ptr<OrderingRules> _ordering = std::make_shared<OrderingRules>();
    std::map<int, std::vector<Hand>> _hand_map;

    /**
     * @brief Convert a hand vector into a map:
     *  key: card value
     *  values: card occurencies
     * 
     * @param hand_arr 
     * @return std::unordered_map<int, int> 
     */
    std::unordered_map<int, int> generate_hand_map(std::vector<int>& hand_v)
    {
        std::unordered_map<int, int> hand_map{};

        for (const auto& card : hand_v) {
            hand_map[card]++;
        }
        return hand_map;
    }
};


/**
 * @brief Map a string of cards into a vector of integers
 * 
 * @param card_str 
 * @return std::vector<int> 
 */
std::vector<int> cards_mapping(std::string card_str)
{
    std::vector<int> card_vec{};

    for (char& ch : card_str) {
        if (ch == 'T') {
            card_vec.push_back(int(11));
        } else if (ch == 'J') {
            card_vec.push_back(int(12));
        } else if (ch == 'Q') {
            card_vec.push_back(int(13));
        } else if (ch == 'K') {
            card_vec.push_back(int(14));
        } else if (ch == 'A') {
            card_vec.push_back(int(15));
        } else {
            auto temp = std::stoi(std::string(1, ch));
            card_vec.push_back(temp);
        }
    }

    return card_vec;
}


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

    if (lines.empty()) {
        std::cerr << "Error: File empty!" << std::endl;
        return lines;
    }

    file.close();

    return lines;
}



int main()
{
    std::cout << "Day 7" << std::endl;

    // Open file
    std::string filename = "data.txt";
    if (TEST) {
        filename = "test_data.txt";
    }
    std::string filepath = "data/" + filename;

    std::vector<std::string> lines = read_file(filepath);

    // Transforming data
    std::vector<Hand> hands_vector{};
    for (const auto& line : lines) {
        auto space_idx = line.find(" ");
        std::string card_str = line.substr(0, space_idx);
        std::string bet_str = line.substr(space_idx+1, line.back());

        Hand hand{};
        hand.cards = cards_mapping(card_str);
        hand.bet = std::stoi(bet_str);

        hands_vector.push_back(hand);
    }

    // Camel game
    std::unique_ptr<CamelGame> camel_game = std::make_unique<CamelGame>(hands_vector);
    camel_game->play();





    return 0;
}
