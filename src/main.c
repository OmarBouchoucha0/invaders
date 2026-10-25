#include <raylib.h>
#include <stdio.h>

typedef struct Ship Ship;
struct Ship {
  Vector2 pos;
  Vector2 dimensions;
  Vector2 corners[3];
};

static int WINDOW_WIDTH = 400;
static int WINDOW_HEIGHT = 600;
static int VELOCITY = 100;

void moveShipLeft(float dt, Ship *ship) {
  ship->pos.x -= dt * VELOCITY;
  ship->corners[0].x = ship->pos.x;
  ship->corners[0].y = ship->pos.y - ship->dimensions.x / 2;
  ship->corners[1].x = ship->pos.x - ship->dimensions.y / 2;
  ship->corners[1].y = ship->pos.y + ship->dimensions.x / 2;
  ship->corners[2].x = ship->pos.x + ship->dimensions.y / 2;
  ship->corners[2].y = ship->pos.y + ship->dimensions.x / 2;
}

void moveShipRight(float dt, Ship *ship) {
  ship->pos.x += dt * VELOCITY;
  ship->corners[0].x = ship->pos.x;
  ship->corners[0].y = ship->pos.y - ship->dimensions.x / 2;
  ship->corners[1].x = ship->pos.x - ship->dimensions.y / 2;
  ship->corners[1].y = ship->pos.y + ship->dimensions.x / 2;
  ship->corners[2].x = ship->pos.x + ship->dimensions.y / 2;
  ship->corners[2].y = ship->pos.y + ship->dimensions.x / 2;
}

void input(float dt, Ship *ship) {
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
    moveShipLeft(dt, ship);
  }
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
    moveShipRight(dt, ship);
  }
}

int main(void) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SPACE INVADERS");
  SetTargetFPS(60);

  Ship ship = {0};
  ship.pos.x = WINDOW_WIDTH / 2;
  ship.pos.y = WINDOW_HEIGHT - 30;
  ship.dimensions.x = 25;
  ship.dimensions.y = 30;
  ship.corners[0].x = ship.pos.x;
  ship.corners[0].y = ship.pos.y - ship.dimensions.x / 2;
  ship.corners[1].x = ship.pos.x - ship.dimensions.y / 2;
  ship.corners[1].y = ship.pos.y + ship.dimensions.x / 2;
  ship.corners[2].x = ship.pos.x + ship.dimensions.y / 2;
  ship.corners[2].y = ship.pos.y + ship.dimensions.x / 2;

  float dt;
  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    input(dt, &ship);
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTriangle(ship.corners[0], ship.corners[1], ship.corners[2], RAYWHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
