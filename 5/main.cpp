#include <cstdint>
#include <iostream>
#include <limits>
#include <sys/types.h>
#include <vector>
#include <set>
#include <cassert>

class DataBase{
public:
    DataBase(const std::string& db_name) : m_name(db_name){}

    void AppendRanges(const uint32_t dest, const uint32_t source, const uint32_t range){
        assert(range >= 1);
        const uint32_t r = range - 1;

        dest_ranges.emplace_back(dest, dest + r);
        source_ranges.emplace_back(source, source + r);
    }

    uint32_t Search(const uint32_t x) const {
        for(size_t i = 0; i < source_ranges.size(); i++){
            const auto result = Within(x, source_ranges[i]);
            if(result.first){
                return dest_ranges[i].first + result.second; 
            }
        }

        return x;
    }

private:
    std::pair<bool, uint32_t> Within(const uint32_t needle, const std::pair<uint32_t,uint32_t>& haystack) const {
        //std::cout << "searching for " << needle << " between " << haystack.first << " and " << haystack.second << std::endl;
        if(needle >= haystack.first && needle <= haystack.second){
            assert(haystack.first <= needle );
            return {true, needle - haystack.first };
        }

        return {false, 0};
    }

    std::string m_name;
    std::vector< std::pair<uint32_t, uint32_t> > dest_ranges{}; 
    std::vector< std::pair<uint32_t, uint32_t> > source_ranges{};

};

enum class Part{
    one,two
};

uint32_t solve(std::istream& in, const Part part){
    std::string dump;
    std::set<uint32_t> seeds{};
    std::vector<std::pair<uint32_t, uint32_t> > seed_ranges{};

    in >> dump;
    if(part == Part::one){
        uint32_t tmp;
        while( in >> tmp) seeds.insert(tmp);
    }else{
        uint32_t a,b;
        while(in >> a >> b) {
            seed_ranges.emplace_back(a,b);
        }
    }

    in.clear();
    std::vector<DataBase> dbs{};

    std::string db_name;
    while(in >> db_name >> dump){
        DataBase next_db{db_name};

        uint32_t dest, source, range;
        while(in >> dest >> source >> range)  next_db.AppendRanges(dest, source, range);

        dbs.push_back(next_db);
        in.clear();
    }

    uint32_t min_local = std::numeric_limits<uint32_t>::max();
    uint32_t c = 0;
    for(const uint32_t seed :  seeds){
        uint32_t next_val = seed;

        for(size_t i = 0; i < dbs.size() ; i++) {
            next_val = dbs[i].Search(next_val);
        }

        min_local= std::min(min_local, next_val);
    }

    return min_local;
}

int main(){
    std::cout << solve(std::cin, Part::one) << std::endl;
    //std::cout << solve(std::cin, Part::two) << std::endl;
}
