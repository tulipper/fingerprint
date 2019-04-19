// myfeature.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include "sector_norm.h"
//#include "imrotate.h"
#include "gabor2d_sub.h"
#include "imgdata.h"
#include "complex.h"
#include "fft.h"
#include <stdlib.h>
#include "readbmp.h"
void mulSpectrums(complex** a, complex** b, complex** out, int height, int width) {
	for(int i = 0; i != height; i ++){
		for (int j = 0; j != width; j ++){
			out[i][j] = multiply(a[i][j], b[i][j]);
		}
	}
}
void zeropadding(dMat* mat, unsigned int top, unsigned int bottom, unsigned int left, unsigned int right) {
	top = left = 0;
	
	mat->array = (double**)realloc(mat->array, (mat->rows + bottom)*sizeof(double*));
	for (int i = mat->rows; i != mat->rows + bottom; i ++)
		mat->array[i] = (double*)calloc(mat->cols + right, sizeof(double));
	for (int i = 0; i != mat->rows + bottom; i ++) {
		//mat->array[i] = (double*) realloc(mat->cols + bottom, sizeof(double));
		mat->array[i] = (double*)realloc(mat->array[i], (mat->cols + right)*sizeof(double));
		for (int j = mat->cols; j != mat->cols+right; j ++) {
			mat->array[i][j] = 0;
		}
	}
	mat->rows += bottom;
	mat->cols += right;
}
dMat myfeature( dMat imagemat)
{
	//1.图像旋转
	// Mat img = imread(namefile,0);
	// if (img.empty())
	// 	cout << "读取图像失败" << endl;
	// imrotate(img, img, 11.25*rotation);	// 旋转角度为正，逆时针旋转

	//2.fingercode特征提取
	// dMat fingerprint = newmat(175, 175);
	// // img.assignTo(fingerprint, CV_64F);
	// for(int i = 0; i != fingerprint.rows; i ++)
	// 	for (int j = 0; j != fingerprint.cols; j++)
	// 		fingerprint.array[i][j] = imgdata[i][j];
	dMat fingerprint = imagemat;
	int num_disk = 8;
	// Mat_<double> myVector;
	// Mat NormalizedPrint;
	dMat myVector = newmat(2,2);
	dMat NormalizedPrint = newmat(2,2);
	printf("before sector_norm\n");
	sector_norm(&fingerprint, 0, 1, &NormalizedPrint, &myVector);
	printf("after sector_norm\n");
	fflush(stdout);
	// Mat_<double> finger_code(72, num_disk, 0.0);
	dMat finger_code = newmat(72, num_disk);

	// dMat z2, z1, ComponentPrint, complexImg, complexImg2, disk, vector;// define when they are first used

	int z1x, z1y, z2x, z2y,px,py;

	for (int angle = 0; angle < num_disk - 1; angle++){
		printf("angle == %d\n", angle);
		dMat z2 = gabor2d_sub(angle, num_disk);
		dMat z1 = newmatcopy(NormalizedPrint);
		//dmatcopy(NormalizedPrint, z1);
		z1x = z1.rows;
		z1y = z1.cols;
		z2x = z2.rows;
		z2y = z2.cols;

		// copyMakeBorder(z1, z1, 0, z2x - 1, 0, z2y - 1, BORDER_CONSTANT, Scalar::all(0));
		zeropadding(&z1,0, z2x-1, 0, z2y-1);
		// Mat planes[] = { z1, Mat::zeros(z1.size(), CV_64F) };
		// merge(planes, 2, complexImg);
		// dft(complexImg, complexImg);
		complex** complexImg = (complex **) calloc( z1.rows, sizeof(complex*));
		for(int i = 0; i != z1.rows; i ++)
			complexImg[i] = (complex*) calloc(z1.cols, sizeof(complex));
		for (int i = 0; i != z1.rows; i ++){
			for (int j = 0; j != z1.cols; j ++) {
				complexImg[i][j].re = z1.array[i][j];
				complexImg[i][j].im = 0.0;
			}
		}
		printf("doing the first fft2\n");
		fflush(stdout);
		FFT2(complexImg, z1.rows, z1.cols);
		//maybe there need to free z1


		zeropadding(&z2, 0, z1x - 1, 0, z1y -1);
		complex** complexImg2 = (complex **) calloc(z2.rows, sizeof(complex*));
		for(int i = 0; i != z2.rows; i ++)
			complexImg2[i] = (complex*) calloc(z2.cols, sizeof(complex));
		for (int i = 0; i != z2.rows; i ++){
			for (int j = 0; j != z2.cols; j ++) {
				complexImg2[i][j].re = z2.array[i][j];
				complexImg2[i][j].im = 0.0;
			}
		}
		printf("doing the second fft2\n");
		FFT2(complexImg2, z2.rows, z2.cols);
		//and there may be need to free z2;


		// copyMakeBorder(z2, z2, 0, z1x - 1, 0, z1y - 1, BORDER_CONSTANT, Scalar::all(0));
		// Mat planes2[] = { z2, Mat::zeros(z2.size(), CV_64F) };
		// merge(planes2, 2, complexImg2);
		// dft(complexImg2, complexImg2);

		mulSpectrums(complexImg, complexImg2, complexImg, z1x + z2x -1, z1y + z2y -1);

		// idft(complexImg, ComponentPrint, DFT_REAL_OUTPUT);
		printf("doing the ifft2");
		IFFT2(complexImg, z1x+z2x-1, z1y+z2y-1);

		px = (z2x - 1 + (z2x - 1) % 2) / 2;
		py = (z2y - 1 + (z2y - 1) % 2) / 2;


		dMat ComponentPrint = newmat(z1x, z1y);
		for(int i = 0; i != ComponentPrint.rows; i ++) {
			for (int j = 0; j != ComponentPrint.cols; j ++) {
				ComponentPrint.array[i][j] = complexImg[i+px][j+py].re;
			}
		}
		
		// ComponentPrint(Rect(py, px, z1y, z1x)).copyTo(ComponentPrint);
		dMat disk = newmat(2,2);
		dMat vector = newmat(2,2);
		sector_norm(&ComponentPrint, 1, 0, &disk, &vector);
		// for (int i = 0; i < num_disk; i++){
		// 	for (int j = 0; j < finger_code.rows; j++)
		// 		// finger_code.(j, i) = vector.at<double>(j, 0);
		// 		finger_code.array[j][i] = vector.array[j][0];
		// }
		for (int i = 0; i != finger_code.rows; i ++)
			finger_code.array[i][angle] = vector.array[i][0];
		//free varibal in the for 
		//free ComplexImg
		for(int i = 0; i != z1.rows; i ++)
			free(complexImg[i]);
		free(complexImg);
		//free complexImg2
		for (int i = 0; i != z2.rows; i ++)
			free(complexImg2[i]);
		free(complexImg2);
		//free other dMat;
		freemat(&z2);
		freemat(&z1);
		freemat(&ComponentPrint);
		freemat(&disk);
		freemat(&vector);
	}
	// free some local varible in the function
	//freemat(&fingerprint);
	freemat(&myVector);
	freemat(&NormalizedPrint);



	return finger_code;

}
// int main () {
// 	dMat imagedata = readbmpfile("1_1.bmp");
// 	if (imagedata.rows ==0 || imagedata.cols == 0 || imagedata.array == NULL) {
// 		printf("read file failed");
// 		return -1;
// 	}
// 	printdMat(&imagedata);
// 	dMat fingercode = myfeature(imagedata);
// 	printdMat(&fingercode);
// 	freemat(&imagedata);
// 	freemat(&fingercode);
// 	return 0;
// }
