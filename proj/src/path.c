#include "path.h"
#include "graphics.h"
#include <string.h>

const char* base = "/home/proj/res/";

char* getPath(char * ficheiro){
	char* screenSize;
	if(get_h_res() == 800){
		screenSize = "800x600/";
	}
	else if(get_h_res() == 1024){
		screenSize = "1024x768/";
	}

	unsigned int len1 = strlen(base);
	unsigned int len2 = strlen(screenSize);
	unsigned int len3 = strlen(ficheiro);
	char *result = (char*)malloc(len1+len2+len3+1);
	memcpy(result, base, len1);
	memcpy(result+len1, screenSize, len2);
	memcpy(result+len1+len2, ficheiro, len3+1);//+1 to copy the null-terminator
	return result;
}
