#include "./headers/constants.h"
#include "./headers/structs.h"
#include "./headers/enums.h"

#include <stdio.h>
#include <stdlib.h>

int getMapLength(const char* map) {
  char ch = map[0];
  int i = 0;
  int length = 0;

  while((ch = map[i]) != '\0') {
    if(ch == '1') length++;
    i++;
  }

  return length;
}

void setFigureColor(Figure* figure) {
  int i;
  Color color;
  switch (figure->type) {
    case 'i':
      color = LIME;
      break;
    case 'o':
      color = DARKPURPLE;
      break;
    case 't':
      color = PURPLE;
      break;
    case 's':
      color = GREEN;
      break;
    case 'z':
      color = RED;
      break;
    case 'l':
      color = ORANGE;
      break;
    case 'j':
      color = BLUE;
      break;
    default:
      return;
      break;
  }

  for(i = 0; i < figure->countBlocks; i++) {
    figure->blocks[i].color = color;
  }
}

Block* getFigureBlocks(const char* map, int length, Vector2 fieldPos) {
  int i, counter, arrIndex;
  char ch;

  int x, y = fieldPos.y;
  Block* blocks = MemAlloc(sizeof(Block) * length);

  for(i = 0, counter = 0, arrIndex = 0; i < strlen(map); i++) {
    x = block_size * counter + fieldPos.x;

    ch = map[i];
    if(ch == '0') {
      counter++;
      continue;
    }
    else if(ch == '1') {
      blocks[arrIndex].rec.x = x;
      blocks[arrIndex].rec.y = y;

      blocks[arrIndex].rec.width = block_size;
      blocks[arrIndex].rec.height = block_size;

      arrIndex++;
      counter++;
    } else if(ch == ' ') {
      y += block_size;
      counter = 0;
    }

  }
  return blocks;
}

int rotateFigure(Figure* figure, Field field) {
  int i, j;
  int minY, maxY, maxX, minX, x, y;
  int newBlocksLength;
  int prevMapIndex;

  Vector2 oldBlocks[figure->countBlocks];
  Block* newBlocks;

  minY = figure->blocks[0].rec.y;
  maxY = figure->blocks[0].rec.y;
  minX = figure->blocks[0].rec.x;
  maxX = figure->blocks[0].rec.x;

  for(i = 0; i < figure->countBlocks; i++) {
    oldBlocks[i] = (Vector2){figure->blocks[i].rec.x, figure->blocks[i].rec.y};
    if(oldBlocks[i].y < minY) minY = oldBlocks[i].y;
    if(oldBlocks[i].y > maxY) maxY = oldBlocks[i].y;
    if(oldBlocks[i].x < minX) minX = oldBlocks[i].x;
    if(oldBlocks[i].x > maxX) maxX = oldBlocks[i].x;
  }

  prevMapIndex = figure->mapIndex;
  figure->mapIndex++;
  if(figure->mapIndex >= figure->mapLength) {
    figure->mapIndex = 0;
  }

  newBlocksLength = getMapLength(figure->maps[figure->mapIndex]);
  newBlocks = getFigureBlocks(figure->maps[figure->mapIndex],
  newBlocksLength, field.pos); 

  x = ((minX + maxX) / 2) / block_size * block_size - block_size;
  y = ((minY + maxY) / 2) / block_size * block_size - block_size;

  for(i = 0; i < newBlocksLength; i++) {
    newBlocks[i].rec.y += ((y / block_size) * block_size);
    newBlocks[i].rec.x += ((x / block_size) * block_size - field.pos.x);
  }
  // CHECK OTHER BLOCK COLIISION
  for(i = 0; i < newBlocksLength; i++) {
    for(j = 0; j < field.occupiedBlocksCount; j++) {
      if(CheckCollisionRecs(newBlocks[i].rec, field.occupiedBlocks[j].rec)) {
        free(newBlocks);
        figure->mapIndex = prevMapIndex;
        return 1;
      }
    }
  }

  // CHECK WALL COLLISION
  for (i = 0; i < newBlocksLength; i++) {
    if (newBlocks[i].rec.x >= field.pos.x + field.size.x)
      for (j = 0; j < newBlocksLength; j++) newBlocks[j].rec.x -= block_size;
    if (newBlocks[i].rec.x < field.pos.x)
      for (j = 0; j < newBlocksLength; j++) newBlocks[j].rec.x += block_size;
    if (newBlocks[i].rec.y + block_size > field.pos.y + field.size.y)
      for (j = 0; j < newBlocksLength; j++) newBlocks[j].rec.y -= block_size;
    if (newBlocks[i].rec.y < field.pos.y)
      for (j = 0; j < newBlocksLength; j++) newBlocks[j].rec.y += block_size;
    }

  free(figure->blocks);
  figure->blocks = NULL;

  figure->blocks = newBlocks;
  figure->countBlocks = newBlocksLength;

  setFigureColor(figure);
  return 0;
}


CollisionType checkFigureCollision(Figure figure, Field field) {
  for (int i = 0; i < figure.countBlocks; i++) {
    Rectangle nextPos = figure.blocks[i].rec;
    nextPos.x += figure.dir.x * block_size;
    nextPos.y += figure.dir.y * block_size;

    if (nextPos.x < field.pos.x || nextPos.x + block_size > field.pos.x + field.size.x) {
      return figure.dir.x < 0 ? c_hor_wall_left : c_hor_wall_right;
    }
    if (nextPos.y + block_size > field.pos.y + field.size.y) return c_down_wall;

    for (int j = 0; j < field.occupiedBlocksCount; j++) {
      if (CheckCollisionRecs(nextPos, field.occupiedBlocks[j].rec)) return c_block;
    }
  }

  return c_none;
}

Figure* createFigure(const char** maps, const char type, int mapsC, Vector2 fieldPos) {
  int i;

  Figure* figure = MemAlloc(sizeof(Figure));
  if (figure == NULL) {
    return NULL;
  }

  figure->mapLength = mapsC;
  figure->mapsLength = MemAlloc(sizeof(int) * mapsC);

  figure->maps = MemAlloc(sizeof(char*) * mapsC);
  
  for(i = 0; i < mapsC; i++) {
    figure->mapsLength[i] = getMapLength(maps[i]);

    figure->maps[i] = MemAlloc(sizeof(char) * (figure->mapsLength[i] + 1));
    if (figure->maps[i] == NULL) {
      fprintf(stderr, "Ошибка: не удалось выделить память для карты.\n");
      return NULL;
    }
    strcpy(figure->maps[i], maps[i]);
  }

  figure->type = type;
  figure->countBlocks = figure->mapsLength[0];
  figure->blocks = getFigureBlocks(figure->maps[0], figure->countBlocks, fieldPos);

  setFigureColor(figure);

  return figure;
}


void moveFigure(Figure* figure, Field field) {
  int i;

  for(i = 0; i < figure->countBlocks; i++) {
    figure->blocks[i].rec.y += figure->dir.y * block_size;
    figure->blocks[i].rec.x += figure->dir.x * block_size;
  }

}

void drawFigure(Figure figure, int maxY, int maxX) {
  int i;
  Block block;
  int offsetShadowY = 1, offsetShadowX = 3;

  for(i = 0; i < figure.countBlocks; i++) {
    block = figure.blocks[i];

    if(block.rec.y + block_size >= maxY) 
      offsetShadowY = 0;
    else offsetShadowY = 3;

    if(block.rec.x + block_size >= maxX) 
      offsetShadowX = 0; 
    else offsetShadowX = 3;
    
    DrawRectangle(block.rec.x + offsetShadowX, block.rec.y + offsetShadowY, block.rec.width, block.rec.height, (Color){0, 0, 0, 60}); // shadow
    DrawRectangle(block.rec.x, block.rec.y, block.rec.width, block.rec.height, block.color);
    DrawRectangleLinesEx(block.rec, 1, WHITE);
  }
}

const char** getFigureMap(const char type) {
  switch (type) {
    case 'i':
      return figure_i;
    case 'o':
      return figure_o;
    case 't':
      return figure_t;
    case 's':
      return figure_s;
    case 'z':
      return figure_z;
    case 'l':
      return figure_l;
    case 'j':
      return figure_j;
    default:
      return NULL;
      break;
  }
}

Figure* getRandomFigure(Vector2 fieldPos) {
  int types[] = {'i','o','t','s','z','l','j'};
  int length = 0;
    
  const char type = types[GetRandomValue(0, 6)];
  const char** maps = getFigureMap(type);
  while(maps[length] != NULL) {
    length++;
  }

  return createFigure(maps, type, length, fieldPos);
}

void freeFigure(Figure* figure) {
  free(figure->blocks);
  free(figure->maps);
  free(figure->mapsLength);
  
  figure = NULL;
}

void drawFigurePath(Figure figure, Field field) {
  int i;

  Figure figurePath;
  Block block;

  figurePath.countBlocks = figure.countBlocks;
  figurePath.blocks = malloc(sizeof(Block) * figure.countBlocks);
  figurePath.dir = (Vector2){0, 1};

  Color color = (Color){0,0,0,25};
  
  for(i = 0; i < figure.countBlocks; i++) {
    figurePath.blocks[i].rec = figure.blocks[i].rec;
  }

  while (checkFigureCollision(figurePath, field) == c_none) {
    for(i = 0; i < figurePath.countBlocks; i++) {
      figurePath.blocks[i].rec.y += block_size;
    }      
  }

  for(i = 0; i < figurePath.countBlocks; i++) {
    if(figure.blocks[i].rec.y >= figurePath.blocks[i].rec.y - block_size) {
      return;
    }
  }

  for(i = 0; i < figurePath.countBlocks; i++) {
    block = figurePath.blocks[i];
    DrawRectangle(block.rec.x, block.rec.y, block.rec.width, block.rec.height, color);
  }

  free(figurePath.blocks);
}