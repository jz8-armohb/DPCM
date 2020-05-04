#include <iostream>
#include "declarations.h"
using namespace std;

void PrintPMF_Entropy(unsigned char* buffer, int qBits, const char* pmfFileName, const char* entrFileName) {
	int count[256] = { 0 };	// Counter
	double freq[256] = { 0 };	// Frequency
	double entropy = 0;

	/* Compute the frequency of each greyscale */
	for (int i = 0; i < w * h; i++) {
		int index = (int)buffer[i];
		count[index]++;
	}

	/* Compute the PMF & entropy */
	for (int i = 0; i < 256; i++) {
		freq[i] = (double)count[i] / (w * h);
		if (freq[i] != 0) {
			entropy += (-freq[i]) * log(freq[i]) / log(2);
		}
	}

	/* Output the stats into a csv file */
	FILE* pmfFilePtr;
	FILE* entrFilePtr;
	if (fopen_s(&pmfFilePtr, pmfFileName, "wb") == 0) {
		cout << "Successfully opened \"" << pmfFileName << "\".\n";
	} else {
		cout << "WARNING!! Failed to open \"" << pmfFileName << "\".\n";
		exit(-1);
	}
	if (fopen_s(&entrFilePtr, entrFileName, "ab") == 0) {
		cout << "Successfully opened \"" << entrFileName << "\".\n";
	} else {
		cout << "WARNING!! Failed to open \"" << entrFileName << "\".\n";
		exit(-1);
	}


	fprintf(pmfFilePtr, "Symbol,Frequency\n");
	for (int i = 0; i < 256; i++) {
		fprintf(pmfFilePtr, "%-3d,%-8.2e\n", i, freq[i]);	// 将数据输出到文件中（csv文件以“,”作为分隔符）
	}
	fprintf(entrFilePtr, "%d,%.4lf\n", qBits, entropy);

	fclose(pmfFilePtr);
	fclose(entrFilePtr);
}

void PrintPSNR(unsigned char* oriBuffer, unsigned char* recBuffer, int qBits, const char* psnrFileName) {
	double mse;
	double sum = 0;
	double temp;
	double psnr;

	for (int i = 0; i < w * h; i++) {
		temp = pow((oriBuffer[i] - recBuffer[i]), 2);
		sum += temp;
	}
	mse = sum / (w * h);
	psnr = 10 * log10(255 * 255 / mse);

	/* Output the stats into a csv file */
	FILE* outFilePtr;
	if (fopen_s(&outFilePtr, psnrFileName, "ab") == 0) {
		cout << "Successfully opened \"" << psnrFileName << "\".\n";
	} else {
		cout << "WARNING!! Failed to open \"" << psnrFileName << "\".\n";
		exit(-1);
	}
	fprintf(outFilePtr, "%d,%lf\n", qBits, psnr);

	fclose(outFilePtr);
}