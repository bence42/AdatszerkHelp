/*
 * introSort.hpp
 *
 *  Created on: Dec 19, 2018
 *      Author: Józsa György Bence
 */

#ifndef INTROSORT_HPP_
#define INTROSORT_HPP_
#include "quicksort.hpp"
#include <cmath>

void _introSort_impl(int * data, int down, int up, int depth) {
}

void introSort(int * data, int N) {
	_introSort_impl(data, 0, N-1, log2(N) * 2);
}



#endif /* INTROSORT_HPP_ */
