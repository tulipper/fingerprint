#pragma once

typedef struct _double_matrix {
	double ** array;
	int rows;
	int cols;
} dMat;
void freemat(dMat* mat); 
void printdMat(dMat* mat); 
dMat newmat(int rows, int cols);
dMat newmatcopy(dMat mat);
void initmat(dMat* mat);
void dmatcopy(dMat* from, dMat* to);