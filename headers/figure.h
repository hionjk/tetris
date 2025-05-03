#include "./enums.h"
#include "./structs.h"

#ifndef FIGURE_H
#define FIGURE_H
Figure* createFigure(const char** maps, const char type, int mapsC, Vector2 fieldPos);
void drawFigure(Figure figure, int maxY, int maxX);
void moveFigure(Figure* figure, Field field);
int rotateFigure(Figure* figure, Field field);
Block* getFigureBlocks(const char* map, int length, Vector2 fieldPos);
CollisionType checkFigureCollision(Figure figure, Field field);
void appendFigureToField(Figure* figure, Field* field);
Figure* getRandomFigure(Vector2 fieldPos);
void freeFigure(Figure* figure);
const char** getFigureMap(const char type);
int getMapLength(const char* map);
void drawFigurePath(Figure figure, Field field);

#endif