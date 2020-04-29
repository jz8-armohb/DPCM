#include <iostream>
#include "declarations.h"
#include <algorithm>
using namespace std;

int main(int argc, char* argv[]) {
    const char* orFileName = "Lena.yuv";
    const char* qpeFileName = "Lena_QPE.yuv";   // Name of quantised prediction error file
    const char* recFileName = "Lena_reconstruction.yuv";    // Name of reconstruction level file
	FILE* oriFilePtr;
    FILE* qpeFilePtr;
	FILE* recFilePtr;

	/* Open the files */
    if (fopen_s(&oriFilePtr, orFileName, "rb") == 0) {
        cout << "Successfully opened \"" << orFileName << "\".\n";
    } else {
        cout << "WARNING!! Failed to open \"" << orFileName << "\".\n";
        exit(-1);
    }
    if (fopen_s(&qpeFilePtr, qpeFileName, "wb") == 0) {
        cout << "Successfully opened \"" << qpeFileName << "\".\n";
    } else {
        cout << "WARNING!! Failed to open \"" << qpeFileName << "\".\n";
        exit(-1);
    }
    if (fopen_s(&recFilePtr, recFileName, "wb") == 0) {
        cout << "Successfully opened \"" << recFileName << "\".\n";
    } else {
        cout << "WARNING!! Failed to open \"" << recFileName << "\".\n";
        exit(-1);
    }

    /* Space allocation */
    unsigned char* oriYBuff = new unsigned char[w * h];
    unsigned char* qpeYBuff = new unsigned char[w * h];
    unsigned char* recYbuff = new unsigned char[w * h];
    unsigned char* uBuff = new unsigned char[w * h / 4];
    unsigned char* vBuff = new unsigned char[w * h / 4];

    /* Read greyscale data */
    fread(oriYBuff, sizeof(unsigned char), w * h, oriFilePtr);

    /* DPCM */
    DpcmEncoding(oriYBuff, qpeYBuff, recYbuff, 4);
    memset(uBuff, 128, w * h / 4);
    memset(vBuff, 128, w * h / 4);
    fwrite(qpeYBuff, sizeof(unsigned char), w * h, qpeFilePtr);
    fwrite(uBuff, sizeof(unsigned char), w * h / 4, qpeFilePtr);    // Greyscale image
    fwrite(vBuff, sizeof(unsigned char), w * h / 4, qpeFilePtr);
    fwrite(recYbuff, sizeof(unsigned char), w * h, recFilePtr);
    fwrite(uBuff, sizeof(unsigned char), w * h / 4, recFilePtr);    // Greyscale image
    fwrite(vBuff, sizeof(unsigned char), w * h / 4, recFilePtr);

    /* Write the PMF & entropy stats into a csv file */
    PMF(oriYBuff, "Lena-PMF.csv");
    PMF(qpeYBuff, "Lena_QPE-PMF.csv");


    fclose(oriFilePtr);
    fclose(qpeFilePtr);
    fclose(recFilePtr);
    delete[]oriYBuff;
    delete[]qpeYBuff;
    delete[]recYbuff;
    delete[]uBuff;
    delete[]vBuff;
}