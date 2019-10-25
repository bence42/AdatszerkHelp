#ifndef KHF03_HEAVY_HITTERS_HPP
#define KHF03_HEAVY_HITTERS_HPP

#include <iterator>
#include <vector>

template <typename ForwardIterator,
          typename ValueType =
              typename std::iterator_traits<ForwardIterator>::value_type>
std::vector<ValueType> heavy_hitters(ForwardIterator begin, ForwardIterator end,
                                     size_t k)
{
}

#endif //KHF03_HEAVY_HITTERS_HPP
