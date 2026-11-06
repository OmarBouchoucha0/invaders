#include <raylib.h>
#include <stdio.h>
#define MAX_ENTITIES 10000
#define PLAYER_ID 1
#define SHIP_SPEED 200.0f
#define BULLET_SPEED 200.0f
#define WINDOW_WIDTH 400.0f
#define WINDOW_HEIGHT 600.0f

#define BULLET_WIDTH 4.0f
#define BULLET_HEIGHT 10.0f;

typedef struct Ship Ship;
struct Ship {
  Vector2 pos;
  Vector2 dimensions;
  Vector2 corners[3];
  Vector2 velocity;
};

typedef struct Bullet Bullet;
struct Bullet {
  Vector2 pos;
  Vector2 dimensions;
  Vector2 velocity;
};

typedef enum State State;
enum State {
  ALIVE,
  DEAD,
};

typedef enum Type Type;
enum Type {
  PLAYER,
  PLAYER_BULLET,
  ENEMY,
  ENEMY_BULLET,
};

typedef struct Entity Entity;
struct Entity {
  union {
    Ship ship;
    Bullet bullet;
  };
  State state;
  Type type;
};

static float FIRE_RATE = 5.0f;
static int LAST_ENTITY_ID = 1;

void addEntity(Entity *entities, Entity entity) {
  for (int i = 1; i < LAST_ENTITY_ID; ++i) {
    if (entities[i].state == DEAD) {
      entities[i] = entity;
      return;
    }
  }
  entities[LAST_ENTITY_ID] = entity;
  LAST_ENTITY_ID++;
}

void spawnPlayer(Entity *entities) {
  Entity player = {0};
  player.type = PLAYER;
  player.state = ALIVE;
  player.ship.pos.x = WINDOW_WIDTH / 2;
  player.ship.pos.y = WINDOW_HEIGHT - 30;
  player.ship.dimensions.x = 15;
  player.ship.dimensions.y = 20;
  addEntity(entities, player);
}

void spawnPlayerBullet(Entity *entities) {
  Entity player = entities[PLAYER_ID];
  Entity bullet = {0};
  bullet.type = PLAYER_BULLET;
  bullet.state = ALIVE;
  bullet.bullet.pos.x = player.ship.pos.x - BULLET_WIDTH / 2.0f;
  bullet.bullet.pos.y =
      player.ship.pos.y - player.ship.dimensions.y / 2.0f - BULLET_HEIGHT;
  bullet.bullet.dimensions.x = BULLET_WIDTH;
  bullet.bullet.dimensions.y = BULLET_HEIGHT;
  bullet.bullet.velocity.y -= BULLET_SPEED;
  addEntity(entities, bullet);
}

void spawnEnemies(Entity *entities) {
  float width = 25;
  float height = 30;
  float spacing = 10.0f;
  int rows = 3;
  int cols = WINDOW_WIDTH / (spacing + width);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      Entity enemy = {0};
      enemy.type = ENEMY;
      enemy.state = ALIVE;
      enemy.ship.pos.x = width + j * (spacing + width);
      enemy.ship.pos.y = height + i * (spacing + height);
      enemy.ship.dimensions.x = width;
      enemy.ship.dimensions.y = height;

      enemy.ship.corners[0].x = enemy.ship.pos.x;
      enemy.ship.corners[0].y = enemy.ship.pos.y + enemy.ship.dimensions.y / 2;
      enemy.ship.corners[1].x = enemy.ship.pos.x + enemy.ship.dimensions.x / 2;
      enemy.ship.corners[1].y = enemy.ship.pos.y - enemy.ship.dimensions.y / 2;
      enemy.ship.corners[2].x = enemy.ship.pos.x - enemy.ship.dimensions.x / 2;
      enemy.ship.corners[2].y = enemy.ship.pos.y - enemy.ship.dimensions.y / 2;
      addEntity(entities, enemy);
    }
  }
}

void input(Entity *entities) {
  Entity *player = &entities[PLAYER_ID];
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
    player->ship.velocity.x -= SHIP_SPEED;
  }
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
    player->ship.velocity.x += SHIP_SPEED;
  }

  if (IsKeyPressed(KEY_SPACE)) {
    spawnPlayerBullet(entities);
  }
}

bool collisionBulletShip(Entity bullet, Entity ship) {
  Vector2 p1 = {bullet.bullet.pos.x, bullet.bullet.pos.y};
  Vector2 p2 = {bullet.bullet.pos.x + bullet.bullet.dimensions.x,
                bullet.bullet.pos.y};
  Vector2 p3 = {bullet.bullet.pos.x + bullet.bullet.dimensions.x,
                bullet.bullet.pos.y + bullet.bullet.dimensions.y};
  Vector2 p4 = {bullet.bullet.pos.x,
                bullet.bullet.pos.y + bullet.bullet.dimensions.y};

  Vector2 v1 = ship.ship.corners[0];
  Vector2 v2 = ship.ship.corners[1];
  Vector2 v3 = ship.ship.corners[2];

  Vector2 collisionPoint = {0};
  Rectangle rect = {
      .x = bullet.bullet.pos.x,
      .y = bullet.bullet.pos.y,
      .width = bullet.bullet.dimensions.x,
      .height = bullet.bullet.dimensions.y,
  };

  bool hit = CheckCollisionLines(v1, v2, p1, p2, &collisionPoint) ||
             CheckCollisionLines(v1, v2, p2, p3, &collisionPoint) ||
             CheckCollisionLines(v1, v2, p3, p4, &collisionPoint) ||
             CheckCollisionLines(v1, v2, p4, p1, &collisionPoint) ||
             CheckCollisionLines(v2, v3, p1, p2, &collisionPoint) ||
             CheckCollisionLines(v2, v3, p2, p3, &collisionPoint) ||
             CheckCollisionLines(v2, v3, p3, p4, &collisionPoint) ||
             CheckCollisionLines(v2, v3, p4, p1, &collisionPoint) ||
             CheckCollisionLines(v3, v1, p1, p2, &collisionPoint) ||
             CheckCollisionLines(v3, v1, p2, p3, &collisionPoint) ||
             CheckCollisionLines(v3, v1, p3, p4, &collisionPoint) ||
             CheckCollisionLines(v3, v1, p4, p1, &collisionPoint) ||
             CheckCollisionPointTriangle(p1, v1, v2, v3) ||
             CheckCollisionPointRec(v1, rect);
  return hit;
}

void collisionBulletEnemy(Entity *entities) {
  for (int i = 1; i < LAST_ENTITY_ID; ++i) {
    if (entities[i].type == PLAYER_BULLET && entities[i].state == ALIVE) {
      Entity *bullet = &entities[i];
      for (int j = 1; j < LAST_ENTITY_ID; ++j) {
        if (entities[j].type == ENEMY && entities[j].state == ALIVE) {
          Entity *enemy = &entities[j];
          if (collisionBulletShip(*bullet, *enemy)) {
            enemy->state = DEAD;
            bullet->state = DEAD;
          }
        }
      }
    }
  }
}

void updatePlayerPos(float dt, Entity *entities) {
  Entity *player = &entities[PLAYER_ID];
  player->ship.pos.x += dt * player->ship.velocity.x;
  player->ship.corners[0].x = player->ship.pos.x;
  player->ship.corners[0].y =
      player->ship.pos.y - player->ship.dimensions.y / 2;
  player->ship.corners[1].x =
      player->ship.pos.x - player->ship.dimensions.x / 2;
  player->ship.corners[1].y =
      player->ship.pos.y + player->ship.dimensions.y / 2;
  player->ship.corners[2].x =
      player->ship.pos.x + player->ship.dimensions.x / 2;
  player->ship.corners[2].y =
      player->ship.pos.y + player->ship.dimensions.y / 2;

  player->ship.velocity.x = 0;
}

void updateBulletsPos(float dt, Entity *entities) {
  for (int i = 0; i < LAST_ENTITY_ID; ++i) {
    Entity *entity = &entities[i];
    if ((entity->type == PLAYER_BULLET || entity->type == ENEMY_BULLET) &&
        entity->state == ALIVE) {
      entity->bullet.pos.y += dt * entity->bullet.velocity.y;
      if (entity->bullet.pos.y > WINDOW_HEIGHT || entity->bullet.pos.y < 0) {
        entity->state = DEAD;
      }
    }
  }
}

void physics(float dt, Entity *entities) {
  updatePlayerPos(dt, entities);
  updateBulletsPos(dt, entities);
  collisionBulletEnemy(entities);
}

void drawEntities(Entity *entities) {
  for (int i = 1; i < LAST_ENTITY_ID; ++i) {
    Entity entity = entities[i];
    if (entity.state == DEAD) {
      continue;
    }
    if (entity.type == PLAYER) {
      DrawTriangleLines(entity.ship.corners[0], entity.ship.corners[1],
                        entity.ship.corners[2], RAYWHITE);
    }
    if (entity.type == PLAYER_BULLET) {
      DrawRectangleLines(entity.bullet.pos.x, entity.bullet.pos.y,
                         entity.bullet.dimensions.x, entity.bullet.dimensions.y,
                         RAYWHITE);
    }
    if (entity.type == ENEMY) {
      DrawTriangle(entity.ship.corners[0], entity.ship.corners[1],
                   entity.ship.corners[2], RAYWHITE);
    }
  }
}

int main(void) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SPACE INVADERS");
  SetTargetFPS(60);
  Entity entities[MAX_ENTITIES] = {0};
  spawnPlayer(entities);
  spawnEnemies(entities);
  float dt = 0.0f;
  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    input(entities);
    physics(dt, entities);
    BeginDrawing();
    ClearBackground(BLACK);
    drawEntities(entities);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
