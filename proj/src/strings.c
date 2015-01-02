#include "strings.h"
#include "graphics.h"
#include "colors.h"
#include "path.h"
#include "bitmap.h"

Alphabet * letters = NULL;
int nextX;

void newAlphabet(){
	Alphabet * a = (Alphabet *)malloc(sizeof(Alphabet));

	a->A = loadBitmap(getPath("letras/A.bmp"));
	a->B = loadBitmap(getPath("letras/B.bmp"));
	a->C1 = loadBitmap(getPath("letras/C.bmp"));
	a->D1 = loadBitmap(getPath("letras/D.bmp"));
	a->E = loadBitmap(getPath("letras/E.bmp"));
	a->F = loadBitmap(getPath("letras/F.bmp"));
	a->G = loadBitmap(getPath("letras/G.bmp"));
	a->H = loadBitmap(getPath("letras/H.bmp"));
	a->I = loadBitmap(getPath("letras/I.bmp"));
	a->J = loadBitmap(getPath("letras/J.bmp"));
	a->K = loadBitmap(getPath("letras/K.bmp"));
	a->L = loadBitmap(getPath("letras/L.bmp"));
	a->M = loadBitmap(getPath("letras/M.bmp"));
	a->N = loadBitmap(getPath("letras/N.bmp"));
	a->O = loadBitmap(getPath("letras/O.bmp"));
	a->P = loadBitmap(getPath("letras/P.bmp"));
	a->Q = loadBitmap(getPath("letras/Q.bmp"));
	a->R = loadBitmap(getPath("letras/R.bmp"));
	a->S1 = loadBitmap(getPath("letras/S.bmp"));
	a->T1 = loadBitmap(getPath("letras/T.bmp"));
	a->U = loadBitmap(getPath("letras/U.bmp"));
	a->V = loadBitmap(getPath("letras/V.bmp"));
	a->W = loadBitmap(getPath("letras/W.bmp"));
	a->X = loadBitmap(getPath("letras/X.bmp"));
	a->Y = loadBitmap(getPath("letras/Y.bmp"));
	a->Z = loadBitmap(getPath("letras/Z.bmp"));
	a->d0 = loadBitmap(getPath("letras/0.bmp"));
	a->d1 = loadBitmap(getPath("letras/1.bmp"));
	a->d2 = loadBitmap(getPath("letras/2.bmp"));
	a->d3 = loadBitmap(getPath("letras/3.bmp"));
	a->d4 = loadBitmap(getPath("letras/4.bmp"));
	a->d5 = loadBitmap(getPath("letras/5.bmp"));
	a->d6 = loadBitmap(getPath("letras/6.bmp"));
	a->d7 = loadBitmap(getPath("letras/7.bmp"));
	a->d8 = loadBitmap(getPath("letras/8.bmp"));
	a->d9 = loadBitmap(getPath("letras/9.bmp"));

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
	case ' ':
		nextX = x + 0.025*get_h_res();
		break;
	case '0':
		bmp = letters->d0;
		break;
	case '1':
		bmp = letters->d1;
		break;
	case '2':
		bmp = letters->d2;
		break;
	case '3':
		bmp = letters->d3;
		break;
	case '4':
		bmp = letters->d4;
		break;
	case '5':
		bmp = letters->d5;
		break;
	case '6':
		bmp = letters->d6;
		break;
	case '7':
		bmp = letters->d7;
		break;
	case '8':
		bmp = letters->d8;
		break;
	case '9':
		bmp = letters->d9;
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
		if(string[i] != '\n' ){
			drawChar(string[i], x+3, y+3, RGB888toRGB565(0,0,255));
			drawChar(string[i],x,y,color);
			i++;
			x = nextX;
		}
	}
}
