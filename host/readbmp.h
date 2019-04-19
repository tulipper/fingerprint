
#pragma once 
#include <stdlib.h>
#include <stdio.h>
#include "Mat.h"
void bmpFileTest(FILE* fpbmp);
/* To get the OffSet of header to data part */
void bmpHeaderPartLength(FILE* fpbmp);
/* To get the width and height of the bmp file */
void BmpWidthHeight(FILE* fpbmp);
/* Show bmp file tagBITMAPFILEHEADER info */
void bmpFileHeader(FILE* fpbmp);
/* Show bmp file tagBITMAPINFOHEADER info */
void bmpInfoHeader(FILE* fpbmp);
/* Show the Data Part of bmp file */
dMat bmpDataPart(FILE* fpbmp);
void BmpBitCount(FILE* fpbmp);
void BmpSizeImage(FILE* fpbmp);
dMat readbmpfile(const char* imagefile);
