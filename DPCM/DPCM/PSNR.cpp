#include <iostream>
#include "declarations.h"
using namespace std;

void PSNR(unsigned char* oriBuffer, unsigned char* recBuffer, int qBits, const char* psnrFileName) {
	double mse;
	double sum = 0;
	double temp;
	double psnr;

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			temp = pow((oriBuffer[i * w + j] - recBuffer[i * w + j]), 2);
			sum += temp;
		}
	}
	mse = sum / (w * h);
	psnr = 10 * log10((pow(2, qBits) - 1) * (pow(2, qBits) - 1) / mse);

	/* Output the stats into a csv file */
	FILE* outFilePtr;
	if (fopen_s(&outFilePtr, psnrFileName, "wb") == 0) {
		cout << "Successfully opened \"" << psnrFileName << "\".\n";
	} else {
		cout << "WARNING!! Failed to open \"" << psnrFileName << "\".\n";
		exit(-1);
	}
	fprintf(outFilePtr, "%lf", psnr);

	fclose(outFilePtr);
}