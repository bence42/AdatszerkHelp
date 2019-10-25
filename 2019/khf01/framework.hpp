#ifndef MANHATTAN_WOODPECKER_FRAMEWORK_HPP
#define MANHATTAN_WOODPECKER_FRAMEWORK_HPP
#include <iostream>

// use these in your solution

struct competitor_t {
    enum direction_t {S, N, E, W};
    int x, y; /**< competitor coordinates */
    direction_t direction; /**< the direction the competitor is facing */
};

std::ostream &operator<<(std::ostream &o, const competitor_t::direction_t &dir) {
  const std::string labels = "SNEW";
  return o << labels[dir];
}

enum direction_t {
    CENTER, /**< either found it or the battery is dead */
    S, N, W, E,
    SE, SW, NW, NE
};

#endif /* ifndef MANHATTAN_WOODPECKER_FRAMEWORK_HPP */
