#include <iostream>
#include "declarations.h"

int w = 256;
int h = 256;

void DpcmEncoding(unsigned char* yBuff, unsigned char* qPredErrBuff, unsigned char* reconBuff) {
	int prediction;
	int predErr;	// Prediction error
	unsigned char invPredErr;	// Inverse quantised value of quantised prediction error

	for (int i = 0; i < h; i++) {
		prediction = 128;	// The prediction of the first pixel of each row set to be 128
		predErr = yBuff[i * w] - prediction;	// predErr ∈ [-128, 128] (8-bit)
		qPredErrBuff[i * w] = predErr + 128;	// Quantised prediction error with the domain of [0,255]
		if (qPredErrBuff[i * w] > 255) {
			qPredErrBuff[i * w] = 255;
		}
		invPredErr = qPredErrBuff[i * w] - 128;
		reconBuff[i * w] = invPredErr + prediction;	// Reconstruction level

		for (int j = 1; j < w; j++) {	// Strat from the second pixel of each row
			prediction = reconBuff[i * w + j - 1];	// The previous pixel value set as prediction
			predErr = yBuff[i * w + j] - prediction;	// predErr ∈ [-255, 255] (9-bit)
			qPredErrBuff[i * w + j] = (predErr + 255) / 2;	// Quantised prediction error ∈ [0, 255]
			invPredErr = qPredErrBuff[i * w + j] * 2 - 255;
			reconBuff[i * w + j] = invPredErr + prediction;	// Reconstruction level
		}
	}
}