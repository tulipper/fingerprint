// gabor2d_sub.cpp.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"
#include "gabor2d_sub.h"
#include "Mat.h"
#include <math.h>
#include <stdlib.h>
const double PI = 3.141592653;
dMat gabor2d_sub(int angle, int num_disk)
{
	int variance = 32;
	int k = 10;

	double x = cos(angle*PI / num_disk);
	double y = sin(angle*PI / num_disk);

	double w = 2 * PI / k;

	// vector<double> sinp(variance + 1, 0.0);
	// vector<double> cosp = sinp;
	//change to the below:
	double* sinp = (double*) calloc(variance+1, sizeof(double));
	double* cosp = (double*) calloc(variance + 1, sizeof(double));

	int tmp = pow((variance + 1), 2);
	// vector<double> xx(tmp, 0.0);
	// vector<double> y_s = xx;
	// vector<double> yy = xx;
	// vector<double> gaborp = xx;
	// Mat_<double> gaborp_2d(variance + 1, variance + 1, 0.0);
	//change to the below:
	double* xx = (double*) calloc(tmp, sizeof(double));
	double* y_s = (double*) calloc(tmp, sizeof(double));	
	double* yy = (double*) calloc(tmp, sizeof(double));
	double* gaborp = (double*) calloc(tmp, sizeof(double));
	dMat gaborp_2d = newmat(variance + 1, variance + 1);
	



	for (int i = -16, p = 0; i <= 16; i++, p++){
		sinp[p] = i*y;
		cosp[p] = i*x;
	}

	int p = 0;
	for (int j = 0; j < 33; j++){
		for (int i = 0; i < 33; i++){
			xx[p] = sinp[i] + cosp[j];
			yy[p] = cosp[i] - sinp[j];
			gaborp[p] = exp(-(pow(xx[p], 2) + pow(yy[p], 2)) / variance)*cos(w*xx[p]);
			gaborp_2d.array[i][j] = gaborp[p];
			p++;
		}
	}
	//free some local varible
	free(xx);
	free(y_s);
	free(yy);
	free(gaborp);
	free(sinp);
	free(cosp);

	return gaborp_2d;
}

