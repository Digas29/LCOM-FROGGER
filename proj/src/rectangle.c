#include "rectangle.h"
#include "mouse.h"

int mouseInsideRec(Rec* rect){
	if(getMouse()->x >= rect->xi &&
			getMouse()->x <= rect->xf &&
			getMouse()->y >= rect->yi &&
			getMouse()->y <= rect->yf){
  		return 1;
	}
	else
		return 0;
}
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
