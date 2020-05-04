#pragma once

/* Global variables */
extern int w;	// Width of image
extern int h;	// Height of image

/* Functions */
void DpcmEncoding(unsigned char* yBuff, unsigned char* qPredErrBuff, unsigned char* reconBuff, int qBits);
void PrintPMF_Entropy(unsigned char* buffer, int qBits, const char* pmfFileName, const char* entrFileName);
void PrintPSNR(unsigned char* oriBuffer, unsigned char* recBuffer, int qBits, const char* psnrFileName);