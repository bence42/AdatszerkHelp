#ifndef QUICKSORT_HPP
#define QUICKSORT_HPP

/// Segéd függvény
void swapint(int& a, int& b) {   // Egészeket cserélő függvény

	a = a + b;                  //Nincs szükség segédváltozóra
	b = a - b;
	a = a - b;
}

/// ========== GYORS RENDEZÉS ==========
//A gyorsrendező felosztó függvénye
int Divide(int* A, int down, int up) {

	int pivot = A[down];        //pivot választás
	int left = down;
	int right = up;

	while (left < right) {
		while (A[left] <= pivot && left < up) { 	//Addig lépegetek előre a baloldalon,
			left = left + 1;          				                //amíg a pivotnál kisebb elemeket találok
		}
		while (A[right] >= pivot && right > down) { //Addig lépegetek hátra a jobboldalon,
			right = right - 1;       				                          //amíg a pivotnál nagyobb elemeket találok
		}
		if (left < right) {
			swapint(A[left], A[right]);
		}
	}
	A[down] = A[right];
	A[right] = pivot;
	return right;
}

void Quicksort(int* A, int down, int up) {

	if (down < up) {
		int q = Divide(A, down, up);
		Quicksort(A, down, q - 1);
		Quicksort(A, q + 1, up);
	}
}

#endif
