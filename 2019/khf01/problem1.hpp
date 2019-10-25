//
// Created by Józsa György Bence on 9/20/2019.
//

#ifndef KHF01_PROBLEM1_HPP
#define KHF01_PROBLEM1_HPP

#include "framework.hpp"

class GridLocator {
private:
    size_t n, k;
public:
    GridLocator(const size_t N, const size_t K) : n(N), k(K) {};

    virtual direction_t check_compass(int x, int y) const = 0;

    std::pair<int, int> search() {
        int x = 0;
        int y = 0;
        unsigned int i;
        for (i = 0; i < k; ++i) {
            switch (check_compass(x, y)) {
                case direction_t::CENTER :
                    return std::make_pair(x, y);
                case direction_t::S :
                    --y;
                    break;
                case direction_t::N :
                    ++y;
                    break;
                case direction_t::W :
                    --x;
                    break;
                case direction_t::E :
                    ++x;
                    break;
                case direction_t::SE :
                    --y;
                    ++x;
                    break;
                case direction_t::SW :
                    x++;
                    y--;
                    break;
                case direction_t::NW :
                    y++;
                    x--;
                    break;
                case direction_t::NE :
                    x++;
                    y++;
                    break;
            }
        }
        return std::make_pair(x, y);
    }
};

#endif //KHF01_PROBLEM1_HPP
