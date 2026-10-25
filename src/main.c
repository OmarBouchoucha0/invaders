#include <raylib.h>
#include <stdio.h>

static int WINDOW_WIDTH = 800;
static int WINDOW_HEIGHT = 450;

int main(void) {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SPACE INVADERS");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("HEllo", 200, 200, 32, RAYWHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
