
#ifndef _SPRITE_H_
#define _SPRITE_H_


typedef struct {
  double x,y;             /**< current sprite position */
  int width, height;   /**< sprite dimensions */
  double xspeed, yspeed;  /**< current speeds in the x and y direction */
  char *map;           /**< the sprite pixmap (use read_xpm()) */
} Sprite;


Sprite * create_sprite(char *pic[]);

void animate_sprite(Sprite *sp, int h_res, int v_res);

void delete_sprite(Sprite *sp);

void destroy_sprite(Sprite *fig);


#endif
