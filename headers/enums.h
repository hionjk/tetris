#include <raylib.h>

#ifndef ENUMS_H
#define ENUMS_H

typedef enum CollisionType 
  {c_hor_wall_left, c_hor_wall_right, c_down_wall, c_top_wall, c_block, c_none} 
CollisionType;

#endif