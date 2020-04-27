#pragma once

/* Global variables */
extern int w;	// Width of image
extern int h;	// Height of image

/* Functions */
void DpcmEncoding(unsigned char* yBuff, unsigned char* qPredErrBuff, unsigned char* reconBuff);