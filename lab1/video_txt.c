#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>
#include <math.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;/* Height of screen in lines */

void vt_fill(char ch, char attr) {
  
	char *vptr;
	vptr = video_mem;
	int i;
	for(i=0; i < scr_width * scr_lines; i++){
		*vptr = ch;
		vptr++;
		*vptr = attr;
		vptr++;
	}
}

void vt_blank() {

	vt_fill(0x00, 0x07);

}

int vt_print_char(char ch, char attr, int r, int c) {
  
	char *vptr;
	vptr = video_mem;
	if(r>=0 && c>=0 && r < scr_lines && c < scr_width) {
		vptr = vptr + r * scr_width * 2 + c * 2;
		*vptr = ch;
		vptr++;
		*vptr = attr;
		return 0;
	}
	else {
		return 1;
	}
}

int vt_print_string(char *str, char attr, int r, int c) {

	char *vptr;
	vptr = video_mem;
	if(r>=0 && c>=0 && r < scr_lines && c < scr_width) {
		vptr = vptr + r * scr_width * 2 + c * 2;
		while(str[0] != 0) {
			*vptr = *str;
			vptr++;
			*vptr = attr;
			vptr++;
			*str++;
		}
		return 0;
	}
	return 1;
}

int vt_print_int(int num, char attr, int r, int c) {
	if(r>=0 && c>=0 && r < scr_lines && c < scr_width){
		char *vptr;
		vptr = video_mem;
		int numDigitos;
		numDigitos = 0;
		int numCopia;
		numCopia = num;
		int digito;
		while(numCopia/10 != 0) {
			numDigitos++;
			numCopia = numCopia/10;
		}
		while(numDigitos != 0) {
			digito = num / pow(10,numDigitos);
			num = num % 10;
			vptr = vptr + r * scr_width * 2 + c * 2;
			*vptr = digito;
			vptr++;
			*vptr = attr;
			vptr++;
			numDigitos--;
		}
		return 0;
	}
	else {
		return 1;
	}
}


int vt_draw_frame(int width, int height, char attr, int r, int c) {

}

/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}
