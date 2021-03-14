#include <stdio.h>
#include <stdlib.h>

#include "calcul.h"

void MultMatrices (int **M1 /*matrice carre 1*/,int **M2 /*matrice carre 2*/,int **R /*matrice de Resultat*/, int size/*la taille de M1 et M2*/){
	for(int i = 0; i < size; ++i)
		for(int j = 0; j < size; ++j){
			R[i][j] = 0;
			for(int k = 0; k < size; ++k)
				R[i][j] += M1[i][k] * M2[k][j];
		}
}
