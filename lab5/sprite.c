#include "video_gr.h"
#include "sprite.h"
#include <stdio.h>

Sprite * create_sprite(char *pic[]) {
	//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
	if( sp == NULL )
		return NULL;
	// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if( sp->map == NULL ) {
		free(sp);
		return NULL;
	}
	return sp;
}

void animate_sprite(Sprite *sp, int h_res, int v_res){
	sp->x = sp->x + sp->xspeed;
	sp->y = sp->y + sp->yspeed;
	if(sp->x < 0){
		sp->x = 0;
		sp->xspeed = - sp->xspeed;
	}
	if(sp->x + sp->width > h_res){
		sp->x = h_res - sp->width;
		sp->xspeed = - sp->xspeed;
	}
	if(sp->y < 0){
		sp->y = 0;
		sp->yspeed = - sp->yspeed;
	}
	if(sp->x + sp->width > h_res){
		sp->y = v_res - sp->height;
		sp->yspeed = - sp->yspeed;
	}
}
void delete_sprite(Sprite *sp){
	int i,j;
	for(i=0;i < sp->height;i++){
		for(j=0;j < sp->width;j++){
			vg_draw_pixel((int)sp->x + j ,(int)sp->y + i,0);
		}
	}
}

void destroy_sprite(Sprite *sp) {
	if( sp == NULL )
		return;
	free(sp->map);
	free(sp);
	sp = NULL;     // hopeless: pointer is passed by value
}
