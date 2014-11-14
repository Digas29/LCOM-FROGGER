#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"

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

void *vg_init(unsigned short mode)
{
	struct reg86u r;
	vbe_mode_info_t info;

	if (vbe_get_mode_info(mode, &info) != 0)
	{
		return NULL;
	}
	h_res=info.XResolution;
	v_res=info.YResolution;
	bits_per_pixel=info.BitsPerPixel;

	int erro;
	struct mem_range mr;

	/* Allow memory mapping */

	unsigned int vram_size = h_res * v_res * (bits_per_pixel/8);

	mr.mr_base = (phys_bytes)(VRAM_PHYS_ADDR);
	mr.mr_limit = mr.mr_base + vram_size;

	if( OK != (erro = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_gr: sys_privctl (ADD_MEM) failed: %d\n", erro);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

	if(video_mem == MAP_FAILED)
		panic("video_gr couldn't map video memory");

	r.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = 1<<14|mode; // set bit 14: linear framebuffer
	r.u.b.intno = 0x10;
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


	return video_mem;
}

int vg_draw_pixel(unsigned short x, unsigned short y, unsigned long color){
	if(x<H_RES && y<V_RES){
		char *vptr;
		vptr = video_mem;
		vptr += y*h_res + x;
		*vptr = color;
		return 0;
	}
	return 1;
}

int vg_draw_rectangle(unsigned short x, unsigned short y, unsigned short width, unsigned short heigth, unsigned long color){
	unsigned int i,j;
	for(i=0;i<width;i++){
		for(j=0;j<width;j++){
			vg_draw_pixel(i+x,j+y,color);
		}
	}
	if ((x + width)<h_res && (y + heigth)<v_res) return 0;
	return 1;
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
