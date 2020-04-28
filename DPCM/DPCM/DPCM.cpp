#include <iostream>
#include "declarations.h"

int w = 256;
int h = 256;

int PixelOverflow(int value, int thLower, int thUpper) {
	if (value < thLower) {
		return thLower;
	} else if (value > thUpper) {
		return thUpper;
	} else {
		return unsigned char(value);
	}
}

void DpcmEncoding(unsigned char* yBuff, unsigned char* qPredErrBuff, unsigned char* reconBuff, int qBits) {
	int prediction;
	int predErr;	// Prediction error
	int invPredErr;	// Inverse quantised value of quantised prediction error

	for (int i = 0; i < h; i++) {
		prediction = 128;	// The prediction of the first pixel of each row set to be 128
		predErr = int(yBuff[i * w]) - prediction;	// predErr with the domain of [-128, 128] (8-bit)
		int temp = (int(predErr) + 128) / int(pow(2, 8 - qBits));
		qPredErrBuff[i * w] = PixelOverflow(temp, 0, pow(2, qBits) - 1);	// (predErr + 128) with the domain of [0, 256]
		invPredErr = int(qPredErrBuff[i * w]) * int(pow(2, 8 - qBits)) - 128;
		reconBuff[i * w] = PixelOverflow(invPredErr + prediction, 0, 255);	// Reconstruction level

		for (int j = 1; j < w; j++) {	// Strat from the second pixel of each row
			prediction = reconBuff[i * w + j - 1];	// The previous pixel value set as prediction
			predErr = int(yBuff[i * w + j]) - prediction;	// predErr with the domain of [-255, 255] (9-bit)
			int temp = (int(predErr) + 255) / int(pow(2, 9 - qBits));
			qPredErrBuff[i * w + j] = PixelOverflow(temp, 0, (pow(2, qBits) - 1));	// (predErr + 255) with the domain of [0, 255]
			invPredErr = int(qPredErrBuff[i * w + j]) * int(pow(2, 9 - qBits)) - 255;
			reconBuff[i * w + j] = PixelOverflow(invPredErr + prediction, 0, 255);	// Reconstruction level

			///* Debug */
			//if ((i == 255) && (j == 217)) {
			//	printf("%d\n %d\n %d\n %d\n %d\n %x\n", prediction, predErr, temp, qPredErrBuff[i * w + j], invPredErr, reconBuff[i * w + j]);
			//}
		}
	}
}