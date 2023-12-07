#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>
#include <cmath>
#include <map>
#include <cassert>

uint32_t solve(std::istream& in){
    uint32_t card_num = 0, sum = 0;
    std::string dump;
    while(!in.eof() && in >> dump >> card_num >> dump){
        uint32_t num = 0;
        std::vector<uint32_t> winning{}, actual{};
        
        while(in >> num) winning.push_back(num);

        in.clear();
        in >> dump;

        while(in >> num) actual.push_back(num);
        in.clear(); 

        std::sort(winning.begin(), winning.end());
        std::sort(actual.begin(), actual.end());
        std::vector<uint32_t> set_intersect{};

        std::set_intersection(winning.begin(), winning.end(), actual.begin(), actual.end(), 
                              std::back_inserter(set_intersect));
        

        double total_points =  set_intersect.size() > 0 ?  std::pow(2,set_intersect.size()-1) : 0;
        sum += static_cast<uint32_t>(total_points);
    }

    return sum;
}

uint32_t solve2(std::istream& in){
    //map each card to the number of cards it wins
    std::map<uint32_t, size_t> card_map;
    uint32_t card_num = 0;
    std::string dump;

    std::queue<uint32_t> card_list{};
    while(!in.eof() && in >> dump >> card_num >> dump){
        uint32_t num = 0;
        std::vector<uint32_t> winning{}, actual{};

        while(in >> num) winning.push_back(num);

        in.clear();
        in >> dump;

        while(in >> num) actual.push_back(num);
        in.clear(); 

        std::sort(winning.begin(), winning.end());
        std::sort(actual.begin(), actual.end());
        std::vector<uint32_t> set_intersect{};

        std::set_intersection(winning.begin(), winning.end(), actual.begin(), actual.end(), 
                              std::back_inserter(set_intersect));


        card_map[card_num] = set_intersect.size();
        card_list.push(card_num);
    }

    uint32_t total_cards = static_cast<uint32_t>(card_list.size());
    while(!card_list.empty()){
        const uint32_t next_card = card_list.front();
        card_list.pop();
        if(next_card == 0) continue;

        assert(card_map.find(next_card) != card_map.end());
        for(uint32_t i = 1; i <= card_map[next_card]; i++){
            const uint32_t winning_card = (next_card + i);

            total_cards += 1;
            card_list.push(winning_card);
        }
    }

    return total_cards;
}

int main(){
    //std::cout << solve(std::cin) << std::endl;
    std::cout << solve2(std::cin) << std::endl;
}
