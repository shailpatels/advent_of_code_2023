#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <cassert>

uint32_t part1(){
    uint32_t sum = 0;
    std::string line{};

    while(!std::cin.eof() && std::cin >> line){
        char a = '\0',b = '\0';

        for(size_t idx = 0; idx != line.size(); idx++){
            if(std::isdigit(line[idx])){
                a = line[idx];
                break;
            }
        }

        for(size_t idx = line.size(); idx > 0; idx--){
            const size_t pos = idx - 1;
            if(std::isdigit(line[pos])){
                b = line[pos];
                break;
            }
        }

        assert(a != '\0' && b != '\0');

        char num[3] = {a, b, '\0'};
        sum += static_cast<uint32_t>(std::stoul(num));
    }

    return sum;
}

uint32_t part2(){
    uint32_t sum = 0;
    std::string line{};

    auto IsDigit = [](char* x) -> std::pair<bool, char>{
        assert(x != nullptr && *x != '\0');

        const std::map<std::string, char> words{
            {"one",  '1'}, 
            {"two",  '2'},
            {"three",'3'}, 
            {"four", '4'},
            {"five", '5'},
            {"six",  '6'}, 
            {"seven",'7'}, 
            {"eight",'8'}, 
            {"nine", '9'}
        };

        if(std::isdigit(x[0])) return {true, x[0]};
        for(const auto& [key, val]: words){
            const std::string in_str = std::string{x};
            for(size_t str_len = 0; str_len <= in_str.size(); str_len++){
                if(std::string{x, x+str_len} == key) return {true, val};
            }
        }

        return {false, '\0'};
    };

    while(!std::cin.eof() && std::cin >> line){
        char a = '\0', b = '\0';
        for(size_t idx = 0; idx != line.size(); idx++){
            const std::pair<bool, char> ret = IsDigit(line.data() + idx); 
            if(ret.first){
                a = ret.second;
                break;
            }
        }

        for(size_t idx = line.size(); idx > 0; idx--){
            const size_t pos = idx - 1;
            const std::pair<bool, char> ret = IsDigit(line.data() + pos); 
            if(ret.first){
                b = ret.second;
                break;
            }
        }

        assert(a != '\0' && b != '\0');

        char num[3] = {a, b, '\0'};
        sum += static_cast<uint32_t>(std::stoul(num));
    }     

    return sum;
}

int main(){
    //std::cout << part1() << std::endl;
    std::cout << part2() << std::endl;
}

