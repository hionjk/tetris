#include "./structs.h"

#ifndef UI_H
#define UI_H
void drawUI(int score, int lines, int figuresPlaced, Figure nextFigure);
void drawGameOverScreen(int score);
void drawWelcomeScreen();
void drawPauseScreen();

Button* createButton(Rectangle rec, Color color, char* text, Vector2 padding);
void drawButton(Button button);
bool checkClick(Button button);
void freeButton(Button* button);
#endif