#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <istream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

class Grid{
public:
    void set(const size_t x, const size_t y, const char val) {
        if(x >= m_grid.size()) m_grid.emplace_back();
        assert(x < m_grid.size());
        if(y >= m_grid[x].size()) m_grid[x].push_back(val);
        assert(y < m_grid[x].size());
        
        m_grid[x][y] = val;
    }

    friend std::ostream& operator<< (std::ostream& out, const Grid& g){
        for(size_t x = 0; x < g.m_grid.size(); x ++){
            for(size_t y = 0; y < g.m_grid[x].size(); y++){
                assert(y < g.m_grid.size());
                assert(x < g.m_grid[y].size());
                out << g.m_grid[y][x];
            }

            out << "\n";
        }

        out << std::endl;
        return out;
    }

    size_t num_rows() const { 
        assert(m_grid.size() > 0);
        return m_grid[0].size();
    }

    size_t num_cols() const {
        return m_grid.size();
    }

    std::vector<char> row(const size_t y) const{
        std::vector<char> ret{};
        for(uint32_t x = 0; x < m_grid.size(); x++){
            ret.push_back(m_grid[x][y]);
        }

        return ret;
    }

    char at(const size_t x, const size_t y) const {
        assert(m_grid.size() > 0);

        if(x >= m_grid.size()) return '.';
        if(y >= m_grid[y].size()) return '.';

        return m_grid[x][y];
    }

    uint32_t get(const size_t x, const size_t y) const {
        return get_with_pos(x, y).first;
    }

    //from any position in a digit on the grid return the number and its start x coord
    std::pair<uint32_t, size_t> get_with_pos(const size_t x, const size_t y) const {
        assert(std::isdigit(at(x,y)));

        //from the start coordinate, move until we see the start of the number
        long int cursor = x;
        do {
            cursor --;
            if(!std::isdigit(at(cursor,y))) {
                cursor += 1;
                break;
            }
        } while(cursor > 0);

        const long int copy = cursor;
        std::string buffer;
        for(; cursor < static_cast<long int>(num_cols()); cursor++){
            const char ch = at(cursor,y);
            if(std::isdigit(ch)){
                buffer += ch;
            }else{
                break;
            }
        }

        assert(buffer.size() > 0);
        return {static_cast<uint32_t>(std::stol(buffer)), copy};
    }

    std::pair<uint32_t, uint32_t> scan_gears(const uint32_t x, const uint32_t y, bool& status) const {
        std::vector<uint32_t> matches{};
        auto isPart = [](const char x) -> bool{
            return (std::isdigit(x));
        };
        
        //check if left or right are nums
        if(x > 0 && isPart(at(x-1,y))) matches.push_back(get(x-1,y));

        if(isPart(at(x+1,y))) matches.push_back(get(x+1,y));

        std::vector<std::pair<uint32_t, size_t> > potential{}; 
        //check up,upleft,upright
        if(y > 0 && isPart(at(x,y-1))) potential.push_back(get_with_pos(x, y-1));
        if(x > 0 && y > 0 && isPart(at(x-1,y-1))) potential.push_back(get_with_pos(x-1, y-1));
        if(y > 0 && isPart(at(x+1,y-1))) potential.push_back(get_with_pos(x+1, y-1));

        //check down,downright,downleft
        if(isPart(at(x,y+1))) potential.push_back(get_with_pos(x,y+1));
        if(x > 0 && isPart(at(x-1,y+1))) potential.push_back(get_with_pos(x-1,y+1));
        if(isPart(at(x+1,y+1))) potential.push_back(get_with_pos(x+1,y+1));

        //go through and duplicate numbers on the top and bottom rows of the gear
        potential.erase(std::unique(potential.begin(), potential.end(), [](auto left, auto right) -> bool{
            return left.second == right.second && left.first == right.first; 
        }), potential.end());

        for(const auto& pot: potential) matches.push_back(pot.first);
        
        if(matches.size() != 2){
            status = false;
            return {0,0};
        }else{
            status = true;
            return { matches[0], matches[1] };
        }
    }

    bool scan_neighors(const std::vector<std::pair<uint32_t, uint32_t> >& positions) const {
        auto isSymbol = [](const char x) -> bool{
            return (!std::isdigit(x)) && (x !=  '.');
        };
    
        for(const auto& pos : positions){
            const uint32_t x = pos.first;
            const uint32_t y = pos.second;

            //left
            if(x > 0 && isSymbol(at(x-1, y))) return true;
            //right 
            if(isSymbol(at(x+1,y))) return true;
            //up
            if(y > 0 && isSymbol(at(x,y-1))) return true;
            //down
            if(isSymbol(at(x,y+1))) return true;
            //up,left
            if(x > 0 && y > 0 && isSymbol(at(x-1,y-1))) return true;
            //up, right
            if(y > 0 && isSymbol(at(x+1,y-1))) return true;
            //down left 
            if(x > 0 && isSymbol(at(x-1, y+1))) return true;
            //down right
            if(isSymbol(at(x+1,y+1))) return true;
        }

        return false;
    }
private:
    std::vector<std::vector<char> > m_grid;
};


std::pair<uint32_t, uint32_t> solve(std::istream& in){
    Grid grid{};
    std::string line;
    uint32_t row_num = 0;

    //parse data into grid
    while(!in.eof() && in >> line){
        for(size_t i = 0; i < line.size(); i++) grid.set(i, row_num, line[i]); 
        row_num++;
    }

        //find numbers
    std::vector<uint32_t> part_nums{}, gear_nums{};
    for(size_t r = 0; r < grid.num_rows(); r++){
        const std::vector<char> row = grid.row(r);
        std::string num_buffer{};

        for(size_t ch = 0; ch < row.size(); ch++){
            const char val = row[ch];
            if(!std::isdigit(val) || (ch + 1 == row.size()) ){
                if(ch + 1 == row.size() && std::isdigit(val)) num_buffer += val; //handle numbers on right side of grid
                //empty
                std::vector<std::pair<uint32_t, uint32_t> > pos{};
                for(size_t buf_idx = 0; buf_idx < num_buffer.size(); buf_idx++){
                    const long int x = ch - 1 - buf_idx;
                    assert(x >= 0);
                    pos.push_back({
                        x,r
                    });
                }

                if(grid.scan_neighors(pos)){
                    part_nums.push_back(std::atoi(num_buffer.c_str()));    
                }

                num_buffer.clear();
            } else {
                //number
                num_buffer += val;
            }         


            //part 2 
            if(val == '*'){
                bool status = false;
                auto result = grid.scan_gears(static_cast<uint32_t>(ch), static_cast<uint32_t>(r), status); 
                if(status){
                    gear_nums.push_back(result.first * result.second);
                }
            }
        }
    }

    //std::cout << grid << "\n";

    uint32_t sum = 0, sum_2 = 0;
    for(size_t i = 0; i < part_nums.size(); i++)
        sum += part_nums[i];
    for(size_t i = 0; i < gear_nums.size(); i++)
        sum_2 += gear_nums[i];

    return {sum, sum_2};
}

int main(){
    const auto result = solve(std::cin);
    std::cout << result.first << "\n" << result.second << std::endl;
}
