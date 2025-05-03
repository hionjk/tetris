#include "./enums.h"
#include "./structs.h"

#ifndef FIELD_H
#define FIELD_H

void drawField(Field field);
Field* initField(int width, int height, int blockSize);
int* checkFilledLines(Field* field);
void removeFieldLine(Field* field, int lineY);
void freeField(Field* field);
void changeFieldLineColor(Field* field, int lineY, Color color);

#endif