#include <iostream>
#include "declarations.h"
using namespace std;

void PMF(unsigned char* buffer, const char* statsFileName) {
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
	FILE* outFilePtr;
	if (fopen_s(&outFilePtr, statsFileName, "wb") == 0) {
		cout << "Successfully opened \"" << statsFileName << "\".\n";
	} else {
		cout << "WARNING!! Failed to open \"" << statsFileName << "\".\n";
		exit(-1);
	}

	fprintf(outFilePtr, "Symbol,Frequency\n");
	for (int i = 0; i < 256; i++) {
		fprintf(outFilePtr, "%-3d,%-8.2e\n", i, freq[i]);	// 将数据输出到文件中（csv文件以“,”作为分隔符）
	}
	fprintf(outFilePtr, "%.4lf", entropy);

	fclose(outFilePtr);
}