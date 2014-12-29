#pragma once

#include "bitmap.h"
#include "stdio.h"

typedef struct {
	Bitmap* A;
	Bitmap* B;
	Bitmap* C1;
	Bitmap* D1;
	Bitmap* E;
	Bitmap* F;
	Bitmap* G;
	Bitmap* H;
	Bitmap* I;
	Bitmap* J;
	Bitmap* K;
	Bitmap* L;
	Bitmap* M;
	Bitmap* N;
	Bitmap* O;
	Bitmap* P;
	Bitmap* Q;
	Bitmap* R;
	Bitmap* S1;
	Bitmap* T1;
	Bitmap* U;
	Bitmap* V;
	Bitmap* W;
	Bitmap* X;
	Bitmap* Y;
	Bitmap* Z;
} Alphabet;

void newAlphabet();
void deleteAlphabet(Alphabet * a);
void drawChar(char caracter, int x, int y, unsigned long color);
void drawString(char string[], int x, int y, unsigned long color);
