//#include "stdafx.h"
#include "whichsector.h"
#include <stdlib.h>
#include "Mat.h"
#include "sector_norm.h"
#include <math.h>
void sector_norm(dMat *img, int mode, int mix, dMat* disk, dMat* myVector){
	// vector<double> image;
	// if (img.isContinuous())
	// 	image.assign((double*)img.datastart, (double*)img.dataend);

	//change to the below:
	double* image = calloc((img->rows) * (img->cols),sizeof(double));
	for(int i = 0; i != (img->rows); i++) {
		for(int j = 0; j != (img->cols); j++)
			image[i*(img->cols) + j] = (img->array)[i][j];
	}

	int N = 175;
	int M = 74;
	int size_m = N*N;

	// vector<double> mean_s(M, 0.0);
	// vector<double> varn_s(M, 0.0);
	// vector<double> num_s(M, 0.0);
	// vector<double> image1(size_m, 0.0); change to the below:


	double * mean_s = calloc(M, sizeof(double));
	double * varn_s = calloc(M, sizeof(double));
	double * num_s = calloc(M, sizeof(double));
	double * image1 = calloc(size_m, sizeof(double));

	int Mo = 50;
	int Vo = 50;

	int tmp;
	for (int i = 1; i <= size_m; i++){
		tmp = whichsector(i) + 1;
		if (tmp >= 1){
			mean_s[tmp - 1] += image[i - 1];
			num_s[tmp - 1] += 1;
		}
	}

	for (int i = 0; i < M; i++){
		mean_s[i] /= num_s[i];
	}

	for (int i = 1; i <= size_m; i++){
		tmp = whichsector(i) + 1;
		if (tmp >= 1)
			varn_s[tmp - 1] += pow((image[i - 1] - mean_s[tmp - 1]), 2);
	}

	for (int i = 0; i < M; i++)
		varn_s[i] /= num_s[i];

	if (mix == 0 || mix == 1){
		for (int i = 1; i <= size_m; i++){
			tmp = whichsector(i) + 1;
			image1[i - 1] = varn_s[tmp - 1];
		}
	}

	if (mode == 0){
		for (int i = 1; i <= size_m; i++){
			tmp = whichsector(i) + 1;
			if ((tmp >= 1) && (abs(varn_s[tmp - 1]>1))){
				if ((image[i - 1] - mean_s[tmp - 1]) < 0){
					if ((tmp == 73 || tmp == 74) && (mix == 0))
						image1[i - 1] = 50;
					else
						image1[i - 1] = Mo - sqrt(Vo / varn_s[tmp - 1] * pow((image[i - 1] - mean_s[tmp - 1]), 2));
				}
				else
				{
					if ((tmp == 73 || tmp == 74) && mix == 0)
						image1[i - 1] = 50;
					else
						image1[i - 1] = Mo + sqrt(Vo / varn_s[tmp - 1] * pow((image[i - 1] - mean_s[tmp - 1]), 2));
				}
			}
			else
			{
				image1[i - 1] = Mo;
			}
		}
		// Mat(image1).reshape(0, N).copyTo(disk);
		// Mat(varn_s).reshape(0, 1).copyTo(myVector);

		freemat(disk);
		disk->rows = N;
		disk->cols = size_m / N;
		disk->array = (double **) calloc(N, sizeof(double*));
		for(int i = 0; i != (disk->rows); i++) {
			disk->array[i] = (double*) calloc(disk->cols, sizeof(double));
		}
		
		for (int i = 0; i != (disk->rows); i++) {
			for(int j = 0; j != (disk->cols); j ++)
				disk->array[i][j] = image1[i * (disk->cols) + j];
		}
		freemat(myVector);
		myVector->rows = 1;
		myVector->cols = M;
		myVector->array = (double **) calloc (myVector->rows, sizeof(double*));
		for(int i = 0; i != myVector->rows; i++)
			myVector->array[i] = (double *) calloc (myVector->cols, sizeof(double));
		for(int i = 0; i != myVector->rows; i ++)
			for(int j = 0; j != myVector->cols; j++)
				myVector->array[i][j] = varn_s[i * myVector->cols + j];

	}
	else
	{
		// Mat(image1).reshape(0, N).copyTo(disk);
		// Mat(varn_s).reshape(0, M).copyTo(myVector);
		freemat(disk);
		disk->rows = N;
		disk->cols = size_m / N;
		disk->array = (double **) calloc(N, sizeof(double*));
		for(int i = 0; i != (disk->rows); i++) {
			disk->array[i] = (double*) calloc(disk->cols, sizeof(double));
		}
		
		for (int i = 0; i != (disk->rows); i++) {
			for(int j = 0; j != (disk->cols); j ++)
				disk->array[i][j] = image1[i * (disk->cols) + j];
		}
		freemat(myVector);
		myVector->rows = M;
		myVector->cols = 1;
		myVector->array = (double **) calloc (myVector->rows, sizeof(double*));
		for(int j = 0; j != myVector->rows; j++)
			myVector->array[j] = (double *) calloc (myVector->cols, sizeof(double));
		for(int i = 0; i != myVector->rows; i ++)
			for(int j = 0; j != myVector->cols; j++)
				myVector->array[i][j] = varn_s[i * myVector->cols + j];
	}
	//free some local varible
	free (image);
	free(mean_s);
	free(num_s);
	free(image1);
	free(varn_s);
}