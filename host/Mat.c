#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
typedef struct _double_matrix {
	double ** array;
	int rows;
	int cols;
} dMat;

void printdMat(dMat* mat) {
	for(int i = 0; i != mat->rows; i++){
		for(int j = 0; j != mat->cols; j++) {
			if (mat->array[i][j] > 1E3 || mat->array[i][j] < 1E-3) {
				printf("%e ", (mat->array)[i][j]);
				if ( (j+1) % (mat-> cols) == 0)
				printf(";\n");
			}
			else
			{
				printf("%f ", (mat->array)[i][j]);
				if ( (j+1) % (mat-> cols) == 0)
				printf(";\n");
			}
			
		}
	}
}
void freemat(dMat* mat) {
	for(int i = 0; i != mat->rows; i++)
		free(mat->array[i]);
	free(mat->array);
}
/*
 *newmat() 输入行数和列数，生成一个值初始化的矩阵；
 * 
*/
dMat newmat(int rows, int cols) {
	dMat mat;
	mat.rows = rows;
	mat.cols = cols;
	mat.array = (double**) calloc(mat.rows, sizeof(double*));
	for(int i = 0; i != mat.rows; i++)
		mat.array[i] = (double*) calloc(mat.cols, sizeof(double));
	return mat;
}
/*
 *initmat()输入一个矩阵的指针，将矩阵之按照序号进行初始化
*/
void initmat(dMat* mat) {
	for(int i = 0; i != mat->rows; i++) 
		for(int j = 0; j != mat->cols; j++) 
			mat->array[i][j] = i * mat->cols + j;
}
void dmatcopy(dMat* from, dMat* to) {
	freemat(to);
	to->rows = from->rows;
	to->cols = from->cols;
	to->array = from->array;
}
dMat newmatcopy(dMat mat){
	dMat ret = newmat(mat.rows, mat.cols);
	for(int i = 0; i != ret.rows; i ++)
		for (int j = 0; j != ret.cols; j ++)
			ret.array[i][j] = mat.array[i][j];
	return ret;
}

