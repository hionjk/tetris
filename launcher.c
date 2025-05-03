#include <raylib.h>
#include <unistd.h>
#include <stdio.h>

const char *game_path = "./tetris";
const char *window_title = "Launcher";
const char *title_on_screen = "TETRIS LAUNCHER";

enum {
  width = 800,
  height = 600,
};

void openGame() {
  execv(game_path, NULL);
  perror(game_path);
  fflush(stderr);
  _exit(1);
}
 
int main() {
  
  InitWindow(width, height, window_title);
  
  while(!WindowShouldClose()) {
    if(IsKeyPressed(KEY_ENTER)) {
      openGame();
    }

    BeginDrawing();
    DrawText(title_on_screen, (width / 2) - MeasureText(title_on_screen, 48) / 2, 100, 48, BLACK);
    DrawText("Press ENTER to start!", (width / 2) - MeasureText("Press ENTER to start!", 32) / 2, height - 100, 32, BLACK);
    ClearBackground(RAYWHITE);
    EndDrawing();
  }

  return 0;
}