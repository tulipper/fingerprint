#include <stdio.h>
#include <stdlib.h>
#include <math.h>
const double CV_PI = 3.141592653;
int whichsector(int index)
{
	int sector_num,r,ring,arc;
	double theta;

	int length = 175;
	int x = div(index, length).rem;
	int y = div(index , length).quot;

	x = x - floor(length / 2);
	y = y - floor(length / 2);

	int rad = x*x + y*y;

	if (rad < 144)
		return	sector_num = 72;

	if (rad >= 5184)
		return sector_num = 73;

	if (x != 0)
		theta = atan(y / (double)x);
	else if (y>0)
		theta = CV_PI / 2;
	else
		theta = -CV_PI / 2;

	if (x < 0)
		theta = theta + CV_PI;
	else if (theta < 0)
		theta += 2 * CV_PI;

	if (theta < 0)
		theta += 2 * CV_PI;

	r = floor(sqrt(rad));
	ring = floor((r - 12) / 10);
	arc = floor(theta / (CV_PI / 6));
	sector_num = ring * 12 + arc;

	return sector_num;
}

