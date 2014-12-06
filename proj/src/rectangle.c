#include "rectangle.h"

Rec* newRec(int xi,int yi, int xf,int yf){
	Rec * rect =(Rec*) malloc(sizeof(Rec));

	rect->xi = xi;
	rect->xf = xf;
	rect->yi = yi;
	rect->yf = yf;

	return rect;
}

void deleteRec(Rec* retangulo){
	free(retangulo);
}
