#include "colors.h"
unsigned long RGB888toRGB565(unsigned short r, unsigned short g, unsigned short b)
{
	return (((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
}
