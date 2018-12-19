#ifndef SORTS_HPP
#define SORTS_HPP

/// Segéd függvények
void swap(int& a, int& b) {             //Hagyományos cserélő függvény
	int tmp = a;                                   //Segédváltozóra van szükség a cseréhez
	a = b;
	b = tmp;
}

/// ========== BUBURÉKRENDEZÉS ==========
void bubbleSort(int* A, int n) {

	for (int j = n - 1; j >= 1; --j) {
		for (int i = 0; i <= j - 1; ++i) {  //Végigjárja előről hátra a tömböt
			if (A[i] <= A[i + 1]) { 		        //Ha az aktuális elem kisebb, mint a következő
				//skip                              	// akkor nem történik semmi
			} else {            	                		//Ha az aktuális elem nagyobb, mint a következő
				swap(A[i], A[i + 1]);  		// akkor megcseréljük a két elemet
			}
		}
	}
}

/// ========== MAXIMUM KIVÁLASZTÁSOS RENDEZÉS ==========
// A rendező Segédfüggvénye: megkeresi a maximális értékű elemet a megadott tartományban (j-ig)
int MaxSel(const int* A, int j) {

	int ind = 0; 				                    //A maximális értékű elem indexe
	for (int i = 0; i < j; ++i) {
		if (A[i + 1] > A[ind]) {
			ind = i + 1;
		}
	}
	return ind;                                     //Visszatér a maximális értékű elem indexével
}

void maxSort(int* A, int n) {

	for (int j = n - 1; j >= 1; --j) { 	    //Végigjárja hátulról előrefelé a tömböt

		int ind = MaxSel(A, j);   		    //Megkeresi a maximumot az elejétõl j-ig
		swap(A[ind], A[j]); 			        //A megtalált maximális elemet kicseréljük az aktuális hátsó elemmel
	}
}

/// ========== BESZÚRÓ RENDEZÉS ==========
void insertionSort(int* A, int n) {

	for (int j = 0; j <= n - 2; ++j) {     //Az 0. indexű elem már rendezett,
		int w = A[j + 1]; 				        //az 1. indexű (j+1) elemtől indulunk, amit eltárolunk átmenetileg (beszúrandó elem)
		int i = j;
		while (i >= 0 && A[i] > w) 		//Megkeressük a 'w' helyét a rendezett szakaszban
		{
			A[i + 1] = A[i];      		            //Jobbra toljuk a 'w'-nél nagyobb elemeket
			i = i - 1;
		}
		A[i + 1] = w;             		            //Beszúrjuk a helyére az elmentett értéket
	}
}

#endif
