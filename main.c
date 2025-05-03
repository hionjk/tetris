#include <raylib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "./headers/constants.h"
#include "./headers/structs.h"
#include "./headers/enums.h"

#include "./headers/field.h"
#include "./headers/figure.h"
#include "./headers/ui.h"

#if defined(PLATFORM_WEB)
  #include <emscripten/emscripten.h>
#endif

static Field* field;
static int score;
static int lines;
static int figuresPlaced;
static float fallingTime;
static float rotateFigureTime;
static float moveFigureTime;

static bool gameOver;
static bool gameStarted;

static bool pause;
static bool musicPaused;
static Figure* figure;
static Figure* nextFigure;

static Music music;
static Sound figureMoveSound;
static Sound figurePlacedSound;
static Sound figureRotateSound;
static Sound figureFallSound;
static Sound figureRotateFailSound;

static Sound lineClearSingleSound;
static Sound lineClearDoubleSound;
static Sound lineClearTripleSound;

static Sound gameOverSound;

static bool linesToDelete;
static int lineFadingTime;
static int* linesToDeleteArr;

static Color fadingColor;

void initMusic() {
  InitAudioDevice();
  music = LoadMusicStream("./audio/Tetris.mp3");
  music.looping = true;
  SetMusicVolume(music, 0.1);
}

void initSounds() {
  figureMoveSound = LoadSound("./audio/figure_move.mp3");
  SetSoundVolume(figureMoveSound, 2);
  figurePlacedSound = LoadSound("./audio/figure_placed.mp3");
  SetSoundVolume(figurePlacedSound, 2);
  figureFallSound = LoadSound("./audio/figure_fall.mp3");
  figureRotateFailSound = LoadSound("./audio/rotate_fail.mp3");
  figureRotateSound = LoadSound("./audio/figure_rotate.mp3");

  lineClearSingleSound = LoadSound("./audio/line_clear_single.mp3");
  lineClearDoubleSound = LoadSound("./audio/line_clear_double.mp3");
  lineClearTripleSound = LoadSound("./audio/line_clear_triple.mp3");

  gameOverSound = LoadSound("./audio/game_over.mp3");
}

void initGame() {
  field = initField(width, height, block_size);
  score = 0;
  fallingTime = 0.0f;
  rotateFigureTime = 0.0f;
  moveFigureTime = 0.0f;

  gameStarted = false;
  gameOver = false;

  pause = false;
  musicPaused = false;
  linesToDelete = false;

  lineFadingTime = 0;
  fadingColor = RED;

  figure = getRandomFigure(field->pos);
  nextFigure = getRandomFigure(field->pos);
}

void restartGame() {
  freeFigure(figure);
  freeField(field);

  initGame();
}

void update() {
  if(!gameStarted) {
    drawWelcomeScreen();

    if(IsKeyPressed(KEY_ENTER)) {
      gameStarted = true;
      PlayMusicStream(music);
    } else {
      return;
    }
  }
  if(gameOver) {
    drawGameOverScreen(score);
    if(IsKeyPressed(KEY_ESCAPE)) {
      exit(0);
    } else if(IsKeyPressed(KEY_ENTER)) {
      restartGame();
    }

    return;
  }
  if(pause) {
    drawPauseScreen();
    if(IsKeyPressed(KEY_ESCAPE)) {
      exit(0);
    } else if(IsKeyPressed(KEY_ENTER)) {
      pause = false;
      ResumeMusicStream(music);
    } else if(IsKeyPressed(KEY_R)) {
      restartGame();
    } else {
      return;
    }
  }

  while(!WindowShouldClose()) {
    if(IsKeyPressed(KEY_P)) {
      pause = true;
      PauseMusicStream(music);
      return;
    }
    
    UpdateMusicStream(music);

    figure->dir = (Vector2){0,0};
    
    if(linesToDelete) {
      BeginDrawing();
      ClearBackground(WHITE); 

      drawField(*field);
      drawUI(score,lines, figuresPlaced, *nextFigure);
      drawFigure(*figure, field_pos_y + field->size.y, field_pos_x + field->size.x);
      EndDrawing();

      lineFadingTime += 1;

      if(lineFadingTime % 8 == 0) {
        fadingColor = ColorToInt(fadingColor) == ColorToInt(RAYWHITE) ? RED : RAYWHITE;
        for(int i = 0; i < count_rows; i++) {
          if(linesToDeleteArr[i] > 0) {
            changeFieldLineColor(field, linesToDeleteArr[i], fadingColor);
            
          }
        }
      }

      if(lineFadingTime >= fading_time) {
        lineFadingTime = 0;
        
        for(int i = 0; i < count_rows; i++) {
          if(linesToDeleteArr[i] > 0) {
            removeFieldLine(field, linesToDeleteArr[i]);
          }
        }

        linesToDelete = false;
        MemFree(linesToDeleteArr);
      } else {
        return;
      }
    }

    if(moveFigureTime >= move_figure_delay) {
      if(IsKeyDown(KEY_LEFT)) {
        figure->dir = (Vector2){-1, 0};
        moveFigureTime = 0;
      } else if(IsKeyDown(KEY_RIGHT)) {
        figure->dir = (Vector2){1, 0};
        moveFigureTime = 0;
      } else if(IsKeyDown(KEY_DOWN)) {
        figure->dir = (Vector2){0, 1};
        fallingTime = 0;
        moveFigureTime = 0;
      }
    }

    if(IsKeyDown(KEY_UP)) {
      if(rotateFigureTime > switch_figure_delay) {
        if(rotateFigure(figure, *field) == 1) {
          PlaySound(figureRotateFailSound);
        } else {
          PlaySound(figureMoveSound);
          rotateFigureTime = 0;
        }
      }  
    }

    rotateFigureTime += GetFrameTime();
    moveFigureTime += GetFrameTime();
    fallingTime += GetFrameTime();

    if(fallingTime >= 1.0f) {
      figure->dir = (Vector2){0, 1};
      fallingTime = 0;
    }

    switch (checkFigureCollision(*figure, *field))
    {
    case c_hor_wall_left:
    case c_hor_wall_right:
      break;
    case c_block:
    case c_down_wall:
      PlaySound(figurePlacedSound);
      appendFigureToField(figure, field);
      freeFigure(figure); 
      figure = nextFigure;
      nextFigure = getRandomFigure(field->pos);

      if(checkFigureCollision(*figure, *field) == c_block) {
        gameOver = true;
        PlaySound(gameOverSound);
        return;
      }

      int filledLines = 0;
      linesToDeleteArr = checkFilledLines(field);

      for(int i = 0; i < count_rows; i++) {
        if(linesToDeleteArr[i] > 0) {
          filledLines++;
        }
      }    

      if(filledLines > 0) {
        switch (filledLines)
        {
        case 1:
          PlaySound(lineClearSingleSound);
          break;
        case 2:
          PlaySound(lineClearDoubleSound);
          break;
        case 3:
          PlaySound(lineClearTripleSound);
          break;
        }
        linesToDelete = true;
        lines += filledLines;
        score += filledLines * 100;
      }
    
      figuresPlaced++;

      break;
    case c_none:
      moveFigure(figure, *field);
    default:
      break;
    }

    if(IsKeyPressed(KEY_M)) {
      if(musicPaused) {
        ResumeMusicStream(music);
      } else {
        PauseMusicStream(music);
      }

      musicPaused = !musicPaused;
    }

    BeginDrawing();
    ClearBackground(WHITE); 

    drawField(*field);
    drawUI(score,lines, figuresPlaced, *nextFigure);
    drawFigure(*figure, field_pos_y + field->size.y, field_pos_x + field->size.x);
    drawFigurePath(*figure, *field);
    EndDrawing();
  }
}

int main() {
  InitWindow(width, height, title);
  initMusic();
  initSounds();
  initGame();
#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
  SetTargetFPS(60);

  while (!WindowShouldClose())
  {
    update();
  }
#endif

}
