#pragma once

typedef struct{
	int xi,yi,xf,yf;
}Rec;

Rec* newRec(int xi,int yi, int xf,int yf);
void deleteRec(Rec* retangulo);
int mouseInsideRec(Rec* rect);
