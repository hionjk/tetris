#include <raylib.h>

#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct block {
  Color color;
  Rectangle rec;
  bool toDelete;
} Block;

typedef struct Figure {
  int countBlocks;
  int mapIndex;
  int mapLength;
  int* mapsLength;
  char** maps;
  Block* blocks;
  Vector2 dir;
  char type;
} Figure;

typedef struct field {
  Vector2 size;
  Vector2 pos;
  int blockCount;
  Block* occupiedBlocks;
  int occupiedBlocksCount;
} Field;

typedef struct Button {
  Rectangle rec;
  Color color;
  char* text;
  Vector2 padding;
} Button;

#endif