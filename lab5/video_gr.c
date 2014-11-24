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
#define END 0xFFFF
#define ONE_MB 0x100000
#define REALPTR(x) (((x) & 0xFFFF) + (((x) & (0xFFFF0000)) >> 12))


/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

extern phys_bytes vram_phisical_address;

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
	vram_phisical_address = info->PhysBasePtr;
	return video_mem;
}

int vg_draw_pixel(unsigned short x, unsigned short y, unsigned long color){
	if(x < h_res && y < v_res){
		char *vptr;
		vptr = video_mem;
		vptr += (y * h_res + x);
		if((*vptr) != color){
			*vptr = color;
		}
		return 0;
	}
	return 1;
}

int vg_draw_rectangle(unsigned short x, unsigned short y, unsigned short width, unsigned short heigth, unsigned long color){
	unsigned int i,j;
	for(i = 0; i < width; i++) {
		for(j = 0; j < heigth; j++){
			vg_draw_pixel(i + x, j + y, color);
		}
	}
	if ((x + width) < h_res && (y + heigth) < v_res) return 0;
	return 1;
}
int vg_draw_line(unsigned short xi,unsigned short xf, unsigned short yi,unsigned short yf, unsigned long color){
	/*
	 * Bresenham's line algorithm
	 * NOTE: this algorithm is not made by us! It's copied from wikipedia!!!
	 * http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C
	 */
	int dx = abs(xf-xi), sx = xi<xf ? 1 : -1;
	int dy = abs(yf-yi), sy = yi<yf ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;

	for(;;){
		vg_draw_pixel(xi,yi,color);
		if (xi==xf && yi==yf) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; xi += sx; }
		if (e2 < dy) { err += dx; yi += sy; }
	}
	return 0;
}

char *read_xpm(char *map[], int *wd, int *ht)
{
	/*
	 * Code provided in the LAB handout
	 */
  int width, height, colors;
  char sym[256];
  int  col;
  int i, j;
  char *pix, *pixtmp, *tmp, *line;
  char symbol;

  /* read width, height, colors */
  if (sscanf(map[0],"%d %d %d", &width, &height, &colors) != 3) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }
#ifdef DEBUG
  printf("%d %d %d\n", width, height, colors);
#endif
  if (width > h_res || height > v_res || colors > 256) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }

  *wd = width;
  *ht = height;

  for (i=0; i<256; i++)
    sym[i] = 0;

  /* read symbols <-> colors */
  for (i=0; i<colors; i++) {
    if (sscanf(map[i+1], "%c %d", &symbol, &col) != 2) {
      printf("read_xpm: incorrect symbol, color at line %d\n", i+1);
      return NULL;
    }
#ifdef DEBUG
    printf("%c %d\n", symbol, col);
#endif
    if (col > 256) {
      printf("read_xpm: incorrect color at line %d\n", i+1);
      return NULL;
    }
    sym[col] = symbol;
  }

  /* allocate pixmap memory */
  pix = pixtmp = malloc(width*height);

  /* parse each pixmap symbol line */
  for (i=0; i<height; i++) {
    line = map[colors+1+i];
#ifdef DEBUG
    printf("\nparsing %s\n", line);
#endif
    for (j=0; j<width; j++) {
      tmp = memchr(sym, line[j], 256);  /* find color of each symbol */
      if (tmp == NULL) {
    	  printf("read_xpm: incorrect symbol at line %d, col %d\n", colors+i+1, j);
    	  return NULL;
      }
      *pixtmp++ = tmp - sym; /* pointer arithmetic! back to books :-) */
#ifdef DEBUG
      printf("%c:%d ", line[j], tmp-sym);
#endif
    }
  }

  return pix;
}

int vg_draw_xpm(unsigned short xi,unsigned short yi, char *map[]){
	int wd;
	int ht;
	char *pic;
	pic = read_xpm(map,&wd,&ht);
	unsigned int i,j;
	for(i = 0; i < ht; i++) {
		for(j = 0; j < wd; j++){
			vg_draw_pixel(xi+j,yi+i,*pic);
			pic++;
		}
	}
	return 0;
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

void draw_sprite(Sprite *sprite){
	int i,j;
	char *bptr;
	bptr = sprite->map;
	for(i = 0; i < sprite->height; i++) {
		for(j = 0; j < sprite->width; j++){
			if((*bptr) != 0){
				vg_draw_pixel((int)sprite->x + j ,(int)sprite->y + i,*bptr);
			}
			bptr++;
		}
	}
}
int controler_info(){
	mmap_t map_info;

	char * virtualBase = (char *) lm_init();
	if(virtualBase == NULL) return 1;

	VESA_INFO *info = (VESA_INFO *)lm_alloc(sizeof(VESA_INFO), &map_info);


	info->VESASignature[0] = 'V';
	info->VESASignature[1] = 'B';
	info->VESASignature[2] = 'E';
	info->VESASignature[3] = '2';


	vbe_get_controler_info(map_info.phys);
	char* ptr;
	ptr = REALPTR(info->VideoModePtr) + virtualBase;

	unsigned short *videoModesPtr = (unsigned short *)ptr;
	printf("Capabilites: \n");
	if(info->Capabilities[0] & BIT(0)){
		printf("DAC width is switchable to 8 bits per primary color\n");
	}
	else{
		printf("DAC is fixed width, with 6 bits per primary color\n");
	}
	if(info->Capabilities[0] & BIT(1)){
		printf("Controller is not VGA compatible\n");
	}
	else{
		printf("Controller is VGA compatible\n");
	}
	if(info->Capabilities[0] & BIT(2)){
		printf("When programming large blocks of information to the RAMDAC, use the blank bit in Function 09h.\n");
	}
	else{
		printf("Normal RAMDAC operation\n");
	}
	printf("\n Modes: \n");
	while(*videoModesPtr != END){
		printf("0x%X \t", *videoModesPtr);
		videoModesPtr++;
	}
	printf("\n\n VRAM SIZE: %d blocks of 64 KB\n", info->TotalMemory);
	lm_free(&map_info);
	return 0;
}
int update_sprite(Sprite *sprite){
	delete_sprite(sprite);
	animate_sprite(sprite, h_res, v_res);
	return 0;
}
