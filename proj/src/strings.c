#include "strings.h"
#include "graphics.h"
#include "colors.h"
#include "bitmap.h"

Alphabet * letters = NULL;
int nextX;

void newAlphabet(){
	Alphabet * a = (Alphabet *)malloc(sizeof(Alphabet));

	a->A = loadBitmap("/home/proj/res/letras/A.bmp");
	a->B = loadBitmap("/home/proj/res/letras/B.bmp");
	a->C1 = loadBitmap("/home/proj/res/letras/C.bmp");
	a->D1 = loadBitmap("/home/proj/res/letras/D.bmp");
	a->E = loadBitmap("/home/proj/res/letras/E.bmp");
	a->F = loadBitmap("/home/proj/res/letras/F.bmp");
	a->G = loadBitmap("/home/proj/res/letras/G.bmp");
	a->H = loadBitmap("/home/proj/res/letras/H.bmp");
	a->I = loadBitmap("/home/proj/res/letras/I.bmp");
	a->J = loadBitmap("/home/proj/res/letras/J.bmp");
	a->K = loadBitmap("/home/proj/res/letras/K.bmp");
	a->L = loadBitmap("/home/proj/res/letras/L.bmp");
	a->M = loadBitmap("/home/proj/res/letras/M.bmp");
	a->N = loadBitmap("/home/proj/res/letras/N.bmp");
	a->O = loadBitmap("/home/proj/res/letras/O.bmp");
	a->P = loadBitmap("/home/proj/res/letras/P.bmp");
	a->Q = loadBitmap("/home/proj/res/letras/Q.bmp");
	a->R = loadBitmap("/home/proj/res/letras/R.bmp");
	a->S1 = loadBitmap("/home/proj/res/letras/S.bmp");
	a->T1 = loadBitmap("/home/proj/res/letras/T.bmp");
	a->U = loadBitmap("/home/proj/res/letras/U.bmp");
	a->V = loadBitmap("/home/proj/res/letras/V.bmp");
	a->W = loadBitmap("/home/proj/res/letras/W.bmp");
	a->X = loadBitmap("/home/proj/res/letras/X.bmp");
	a->Y = loadBitmap("/home/proj/res/letras/Y.bmp");
	a->Z = loadBitmap("/home/proj/res/letras/Z.bmp");

	letters = a;
}
void deleteAlphabet(Alphabet * a){
	free(a);
}

void drawChar(char caracter, int x, int y, unsigned long color){
	if(letters == NULL){
		return;
	}
	unsigned long MS = color >> 8;
	unsigned long LS = color & 0x11111111;
	Bitmap * bmp = NULL;

	switch(caracter){
	case 'a':
		bmp = letters->A;
		break;
	case 'b':
		bmp = letters->B;
		break;
	case 'c':
		bmp = letters->C1;
		break;
	case 'd':
		bmp = letters->D1;
		break;
	case 'e':
		bmp = letters->E;
		break;
	case 'f':
		bmp = letters->F;
		break;
	case 'g':
		bmp = letters->G;
		break;
	case 'h':
		bmp = letters->H;
		break;
	case 'i':
		bmp = letters->I;
		break;
	case 'j':
		bmp = letters->J;
		break;
	case 'k':
		bmp = letters->K;
		break;
	case 'l':
		bmp = letters->L;
		break;
	case 'm':
		bmp = letters->M;
		break;
	case 'n':
		bmp = letters->N;
		break;
	case 'o':
		bmp = letters->O;
		break;
	case 'p':
		bmp = letters->P;
		break;
	case 'q':
		bmp = letters->Q;
		break;
	case 'r':
		bmp = letters->R;
		break;
	case 's':
		bmp = letters->S1;
		break;
	case 't':
		bmp = letters->T1;
		break;
	case 'u':
		bmp = letters->U;
		break;
	case 'v':
		bmp = letters->V;
		break;
	case 'w':
		bmp = letters->W;
		break;
	case 'x':
		bmp = letters->X;
		break;
	case 'y':
		bmp = letters->Y;
		break;
	case 'z':
		bmp = letters->Z;
		break;
	default:
		break;
	}

	if (bmp == NULL)
		return;

	int width = bmp->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = bmp->bitmapInfoHeader.height;

	if (x + width < 0 || x > get_h_res() || y + height < 0
			|| y > get_v_res())
		return;


	char* bufferStartPos;
	char* imgStartPos;

	int i,j;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		if (pos < 0 || pos >= get_v_res())
			continue;

	 	bufferStartPos = (char*)getBuffer();
		bufferStartPos += x * 2 + pos * get_h_res() * 2;

 		imgStartPos = bmp->bitmapData + i * width * 2;

		for(j = 0; j < width; j++){
			if((*imgStartPos) != 0 || (*(imgStartPos+1)) != 0){
				*bufferStartPos = LS;
				*(bufferStartPos+1) = MS;
			}
			bufferStartPos++;
			bufferStartPos++;
			imgStartPos++;
			imgStartPos++;
		}
	}
	nextX = x + width;
}
void drawString(char string[], int x, int y, unsigned long color){
	int i = 0;
	while(string[i] != '\0'){
		drawChar(string[i], x+3, y+3, RGB888toRGB565(0,0,255));
		drawChar(string[i],x,y,color);
		i++;
		x = nextX;
	}
}
