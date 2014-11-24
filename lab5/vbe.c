#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14
#define BIOS_SERVICE 0x10
#define VBE_GET_MODE_INFO 0x4F01
#define VBE_GET_INFO 0x4F00


#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {


	mmap_t map_info;

	char * virtualBase = (char *) lm_init();

	if(virtualBase == NULL) return 1;

	if (lm_alloc(sizeof(vbe_mode_info_t), &map_info) == NULL) {
		printf("Allocation of memory error\n");
		return 1;
	}

	struct reg86u r;

	r.u.w.ax = VBE_GET_MODE_INFO;
	r.u.w.es = PB2BASE(map_info.phys);
	r.u.w.di = PB2OFF(map_info.phys);
	r.u.w.cx = mode;
	r.u.b.intno = BIOS_SERVICE;

	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return 1;
	}
	switch(r.u.b.ah){
	case 0x01:
		printf("Function call failed \n");
		return 1;
		break;
	case 0x02:
		printf("Function is not supported in current HW configuration \n");
		return 1;
		break;
	case 0x03:
		printf("Function is invalid in current video mode \n");
		return 1;
		break;
	default:
		break;
	}
	memcpy(vmi_p,map_info.virtual, sizeof(vbe_mode_info_t));
	lm_free(&map_info);

	return 0;
}
int vbe_get_controler_info(phys_bytes address) {

	struct reg86u r;
	r.u.w.ax = VBE_GET_INFO;
	r.u.w.es = PB2BASE(address);
	r.u.w.di = PB2OFF(address);
	r.u.b.intno = BIOS_SERVICE;

	if( sys_int86(&r) != OK ) {
		printf("get_vbe_info: sys_int86() failed \n");
		return 1;
	}
	switch(r.u.b.ah){
	case 0x01:
		printf("Function call failed \n");
		return 1;
		break;
	case 0x02:
		printf("Function is not supported in current HW configuration \n");
		return 1;
		break;
	case 0x03:
		printf("Function is invalid in current video mode \n");
		return 1;
		break;
	default:
		break;
	}

	return 0;
}
