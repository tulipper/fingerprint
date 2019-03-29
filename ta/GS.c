//#include "stdafx.h"

#include "GS.h"
#include "tee_math.h"
#include "normrnd.h"
#define row 24
#define column 24

void GS(double A [24][24],double V [24][24]){
	//const int row = 24;
	//const int column = 24;
	double v [24][24] = {0};
	double NORM = 0;
	double h [column - 1] [column] =  {0};
	int Num = 0; //��������
	for (int m = 0; m < row; m++)
		NORM += A[m][0] * A[m][0];
	NORM = tee_sqrt(NORM);

	for (int m = 0; m < row; m++){
		v[m][0] = A[m][0] / NORM;
	}
	//int Num = 0; //��������
	//double h [column - 1] [column] =  {0.0};
	for (int j = 1; j < column; j++){
		double NORMj = 0;
		double sum [row] [1] = {0};
		for (int i = 0; i < j; i++){
			for (int m = 0; m < row; m++)
				h[i][j] += A[m][j] * v[m][i];
			for (int m = 0; m < row; m++)
				sum[m][0] += h[i][j] * v[m][i];
				//sum(m, 0) += h(i, j)*v(m, i);
			Num++;
		}

		for (int m = 0; m < row; m++)
			v[m][j] = A[m][j] - sum [m][0];
			//v(m, j) = A.at<double>(m, j) - sum(m, 0);

		//double NORMj = 0;
		for (int m = 0; m < row; m++)
			NORMj += v[m][j] * v[m][j];
			//NORMj += v(m, j)*v(m, j);
		NORMj = tee_sqrt(NORMj);

		for (int m = 0; m < row; m++)
			v[m][j] /= NORMj;
			//v(m, j) /= NORMj;
	}
	//v.copyTo(V);
	MatrixCopy(V,v);
}
