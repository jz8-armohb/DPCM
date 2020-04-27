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
		qPredErrBuff[i * w] = predErr / 2 + 64;	// 8-bit quantisation of prediction error (quantised prediction error ∈ [0, 128])
		invPredErr = (qPredErrBuff[i * w] - 64) * 2;
		reconBuff[i * w] = invPredErr + prediction;	// Reconstruction level

		for (int j = 1; j < w; j++) {	// Strat from the second pixel of each row
			prediction = reconBuff[i * w + j - 1];	// The previous pixel value set as prediction
			predErr = yBuff[i * w + j] - prediction;	// predErr ∈ [-255, 255] (9-bit)
			qPredErrBuff[i * w + j] = predErr / 2 + 127;	// Quantised prediction error ∈ [0, 254]
			invPredErr = (qPredErrBuff[i * w + j] - 127) * 2;
			reconBuff[i * w + j] = invPredErr + prediction;	// Reconstruction level
		}
	}
}