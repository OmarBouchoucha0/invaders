#include <raylib.h>
#include <stdio.h>

typedef struct Ship Ship;
struct Ship {
  Vector2 pos;
  Vector2 dimensions;
  Vector2 corners[3];
};

typedef struct Bullets Bullets;
struct Bullets {
  Vector2 *pos;
  size_t count;
};
typedef enum EnemiesState EnemiesState;
enum EnemiesState {
  ALIVE = 0,
  DEAD,
};

typedef struct Enemies Enemies;
struct Enemies {
  Ship *ships;
  size_t count;
  EnemiesState enemeis_state;
};

static int WINDOW_WIDTH = 400;
static int WINDOW_HEIGHT = 600;
static int VELOCITY = 100;
static int BULLET_VELOCITY = 200;
static float FIRE_RATE = 5.0f;
static float BULLET_WIDTH = 4.0f;
static float BULLET_HEIGHT = 10.0f;

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

void shoot(float dt, float *time_since_last_shot, Ship ship, Bullets *bullets) {
  bool shot = true;
  if (*time_since_last_shot >= 1 / FIRE_RATE) {
    *time_since_last_shot = 0.0f;
    shot = false;
  }
  for (int i = 0; i < bullets->count; i++) {
    bullets->pos[i].y -= dt * BULLET_VELOCITY;
    if (bullets->pos[i].y < 0 && !shot) {
      shot = true;
      bullets->pos[i].y = ship.pos.y - ship.dimensions.y / 2;
      bullets->pos[i].x = ship.pos.x - (BULLET_WIDTH / 2.0f);
      bullets->count += 1;
    }
  }
  if (!shot) {
    bullets->pos[bullets->count].y = ship.pos.y - ship.dimensions.y / 2;
    bullets->pos[bullets->count].x = ship.pos.x - (BULLET_WIDTH / 2.0f);
    bullets->count += 1;
  }
}

void drawBullets(Bullets bullets) {
  for (int i = 0; i < bullets.count; i++) {
    DrawRectangle(bullets.pos[i].x, bullets.pos[i].y, BULLET_WIDTH,
                  BULLET_HEIGHT, RAYWHITE);
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

  Bullets bullets = {0};
  Vector2 pos[1000];
  bullets.pos = pos;
  float dt = 0.0f;
  float time_since_last_shot = 0.0f;
  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    time_since_last_shot += dt;
    input(dt, &ship);
    shoot(dt, &time_since_last_shot, ship, &bullets);
    BeginDrawing();
    ClearBackground(BLACK);
    drawBullets(bullets);
    DrawTriangle(ship.corners[0], ship.corners[1], ship.corners[2], RAYWHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
