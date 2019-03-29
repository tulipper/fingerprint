#ifndef __NORMRND_H__
#define __NORMRND_H__

extern void normrnd(double aver, double sigma, int row, int column, double a [24][24]);
void srand (unsigned seed);
int rand (void);
void MatrixCopy(double dest [24][24], double src [24][24]);
void transMatrix(double mat [24][24]);
#endif
