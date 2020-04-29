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
		predErr = yBuff[i * w] - prediction;	// predErr with the domain of [-128, 128] (8-bit)
		int temp = (predErr + 128) / pow(2, 8 - qBits);	// qBits-bit quantisation
														// (predErr + 128) with the domain of [0, 256]
		qPredErrBuff[i * w] = PixelOverflow(temp, 0, pow(2, qBits) - 1);
		invPredErr = qPredErrBuff[i * w] * pow(2, 8 - qBits) - 128;	// Inverse quantisation
		reconBuff[i * w] = PixelOverflow(invPredErr + prediction, 0, 255);	// Reconstruction level

		for (int j = 1; j < w; j++) {	// Strat from the second pixel of each row
			prediction = reconBuff[i * w + j - 1];	// The previous pixel value set as prediction
			predErr = yBuff[i * w + j] - prediction;	// predErr with the domain of [-255, 255] (9-bit)
			int temp = (predErr + 255) / pow(2, 9 - qBits);	// qBits-bit quantisation
															// (predErr + 255) with the domain of [0, 510]; [0, 2^(qBits) - 1] after division
			qPredErrBuff[i * w + j] = PixelOverflow(temp, 0, (pow(2, qBits) - 1));	// (predErr + 255) with the domain of [0, 255]
			invPredErr = qPredErrBuff[i * w + j] * pow(2, 9 - qBits) - 255;
			reconBuff[i * w + j] = PixelOverflow(invPredErr + prediction, 0, 255);	// Reconstruction level
		}
	}
}