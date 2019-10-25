//
// Created by Józsa György Bence on 9/27/2019.
//

#ifndef KHF01_PROBLEM2_HPP
#define KHF01_PROBLEM2_HPP

#include <utility>
#include <map>
#include <algorithm>
#include "framework.hpp"

std::pair<int, int> search(const competitor_t *competitors, const size_t N, const size_t K)
{
    std::map< std::pair<int,int>, int > mapComp;
    auto maxi = std::make_pair(0,0);
    for(unsigned int i = 0; i<K ;++i){
        if(competitors[i].x < int(N-1) || competitors[i].y < int(N-1)) {
            auto temp = std::make_pair(competitors[i].x, competitors[i].y);
            switch (competitors[i].direction) {
                case competitor_t::direction_t::S :
                    temp.second++;
                    break;
                case competitor_t::direction_t::N :
                    temp.second--;
                    break;
                case competitor_t::direction_t::E :
                    temp.first++;
                    break;
                case competitor_t::direction_t::W :
                    temp.first--;
                    break;
            }
            ++mapComp[temp];
            maxi = std::max_element(mapComp.begin(), mapComp.end(), mapComp.value_comp())->first;
        }
    }
    return maxi;
}

#endif //KHF01_PROBLEM2_HPP
