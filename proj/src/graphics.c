#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vbe.h"
#include "lmlib.h"
#include "sprite.h"

#define LINEAR_MODEL_BIT 14
#define BIOS_SERVICE 0x10
#define SET_MODE 0x4F02
#define BIT(n) (0x01 << (n))
#define REALPTR(x) (((x) & 0xFFFF) + (((x) & (0xFFFF0000)) >> 12))


/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */
static char *mouseBuffer; /* Mouse Buffer */
static char *buffer; /* Third buffer */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */


void *vg_init(unsigned short mode)
{
	struct reg86u r;
	vbe_mode_info_t *info = malloc(sizeof(vbe_mode_info_t));

	if (vbe_get_mode_info(mode, info) != 0)
	{
		return NULL;
	}
	h_res=info->XResolution;
	v_res=info->YResolution;
	bits_per_pixel=info->BitsPerPixel;

	printf("%d\n", h_res);

	int erro;
	struct mem_range mr;

	/* Allow memory mapping */

	unsigned int vram_size = h_res * v_res * (bits_per_pixel/8);

	mr.mr_base = (phys_bytes)(info->PhysBasePtr);
	mr.mr_limit = mr.mr_base + vram_size;

	if( OK != (erro = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_gr: sys_privctl (ADD_MEM) failed: %d\n", erro);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

	if(video_mem == MAP_FAILED)
		panic("video_gr couldn't map video memory");

	r.u.w.ax = SET_MODE; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = BIT(LINEAR_MODEL_BIT)|mode; // set bit 14: linear framebuffer
	r.u.b.intno = BIOS_SERVICE;
	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}
	switch(r.u.b.ah){
	case 0x01:
		printf("Function call failed \n");
		return NULL;
		break;
	case 0x02:
		printf("Function is not supported in current HW configuration \n");
		return NULL;
		break;
	case 0x03:
		printf("Function is invalid in current video mode \n");
		return NULL;
		break;
	default:
		break;
	}
	mouseBuffer = malloc(vram_size);
	buffer = malloc(vram_size);
	return video_mem;
}

int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}

int buffer_draw_pixel(unsigned short x, unsigned short y, unsigned long color){
	if(x < h_res && y < v_res){
		char *vptr;
		vptr = buffer;
		vptr += (y * h_res + x)*(bits_per_pixel/8);
		while(color != 0){
			unsigned long temp = color & 0xFF;
			color >>= 8;
			if((*vptr) != temp){
				*vptr = temp;
			}
			vptr++;
		}
		return 0;
	}
	return 1;
}

int buffer_draw_line(unsigned short xi,unsigned short xf, unsigned short yi,unsigned short yf, unsigned long color){
	/*
	 * Bresenham's line algorithm
	 * NOTE: this algorithm is not made by us! It's copied from wikipedia!!!
	 * http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C
	 */
	int dx = abs(xf-xi), sx = xi<xf ? 1 : -1;
	int dy = abs(yf-yi), sy = yi<yf ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;

	for(;;){
		buffer_draw_pixel(xi,yi,color);
		if (xi==xf && yi==yf) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; xi += sx; }
		if (e2 < dy) { err += dx; yi += sy; }
	}
	return 0;
}


void buffer_draw_bmp(Sprite *sprite){
	int i,j;
	char *bptr;
	bptr = sprite->map;
	for(i = 0; i < sprite->height; i++) {
		for(j = 0; j < sprite->width; j++){
			if((*bptr) != 0){
				buffer_draw_pixel((int)sprite->x + j ,(int)sprite->y + i,*bptr);
			}
			bptr++;
		}
	}
}
int update_bmp(Sprite *sprite){
	delete_sprite(sprite);
	animate_sprite(sprite, h_res, v_res);
	return 0;
}


void flipMouseBuffer(){
	memcpy(mouseBuffer,buffer,h_res*v_res*(bits_per_pixel/8));
}

void flipVRAM(){
	memcpy(video_mem,mouseBuffer,h_res*v_res*(bits_per_pixel/8));
}
int get_h_res(){
	return h_res;
}
int get_v_res(){
	return v_res;
}
char* getBuffer(){
	return buffer;
}
char* getMouseBuffer(){
	return mouseBuffer;
}

int bufffer_draw_rectangle(unsigned short x, unsigned short y, unsigned short width, unsigned short heigth, unsigned long color){
	unsigned int i,j;
	for(i = 0; i < width; i++) {
		for(j = 0; j < heigth; j++){
			buffer_draw_pixel(i + x, j + y, color);
		}
	}
	if ((x + width) < h_res && (y + heigth) < v_res) return 0;
	return 1;
}
