
#include "tee_math.h"
#include "normrnd.h"
const double PI = 3.1415926;
static unsigned long next = 0;
const int RAND_MAX = 0x7fff;

void srand (unsigned seed) {
	next = (unsigned long) seed;
}
int rand (void) {
	return( ((next = next*214013L+2531011L) >> 16) &0x7fff);
}
void normrnd(double aver, double sigma, int row, int column, double a [24][24])
{
	double x, dScope, y, fengzhi;
	int i, j;
	/*　处理可能未使用的参数*/
	row = row;
	column = column;
	srand((unsigned)rand());

	for (i = 0; i<24; i++)
	{
		for (j = 0; j<24; j++)
		{
			do
			{
				x = ((double)rand() / RAND_MAX) * 6 * sigma + aver - 3 * sigma;  //产生均值正负3sigma的值，
				y = 1.0 / (tee_sqrt(2 * PI)*sigma)*tee_exp(-1 * (x - aver)*(x - aver) / (2 * sigma*sigma));
				fengzhi = 1.0 / (tee_sqrt(2 * PI)*sigma);
				dScope = ((double)rand() / RAND_MAX)*fengzhi;
			} while (dScope > y);
		

			a[i][j] = x;
		}
	}
	//p.copyTo(R);


}
void MatrixCopy(double dest [24][24], double src [24][24]) {
	for (int i = 0; i != 24; i ++)
		for (int j = 0 ; j != 24; j ++)
			dest [i][j] = src [i][j];
}
void transMatrix(double mat [24][24]){
	for (int i = 0; i != 24; i ++) {
		for (int j = i; j != 24; j ++) {
			double tmp = mat [i][j];
			mat [i][j] = mat [j][i];
			mat [i][j] = tmp;
		}
	}
}























