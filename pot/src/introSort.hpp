/*
 * introSort.hpp
 *
 *  Created on: Dec 19, 2018
 *      Author: Józsa György Bence
 */

#ifndef INTROSORT_HPP_
#define INTROSORT_HPP_
#include "quicksort.hpp"
#include "heap_sort.hpp"
#include <cmath>
#include <vector>
#include<stdio.h>

void insertionSort(int* A, int n) {

	for (int j = 0; j <= n - 2; ++j) {     //Az 0. indexű elem már rendezett,
		int w = A[j + 1]; //az 1. indexű (j+1) elemtől indulunk, amit eltárolunk átmenetileg (beszúrandó elem)
		int i = j;
		while (i >= 0 && A[i] > w) //Megkeressük a 'w' helyét a rendezett szakaszban
		{
			A[i + 1] = A[i];      	//Jobbra toljuk a 'w'-nél nagyobb elemeket
			i = i - 1;
		}
		A[i + 1] = w;             	//Beszúrjuk a helyére az elmentett értéket
	}
}

bool _isSorted(int a[], int end) {
	for (int i = 0; i < end; i++) {
		if (a[i] > a[i + 1]) {
			return false;
		} else {
			return true;
		}
	}
	return true;
}

void _introSort_impl(int * data, int down, int up, int depth) {
	std::vector<int> temp;

	while (up - down > 16) {
		if (depth == 0) {
			for (int i = down; i <= up; i++) {
				temp.push_back(data[i]);
			}
			heapSort(temp);
			for(int i = 0; i < temp.size(); i++) {
				data[down+i] = temp[i];
			}
		} else {
			int pivot;
			if (_isSorted(data, up))
				break;
			pivot = Divide(data, down, up);
			_introSort_impl(data, pivot + 1, up, depth - 1);
			up = pivot - 1;
		}
		temp.clear();
	}
}

void introSort(int * data, int N) {
	_introSort_impl(data, 0, N - 1, log2(N) * 2);
	insertionSort(data, N);
}

#endif /* INTROSORT_HPP_ */
