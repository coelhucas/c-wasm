#include "vendor/raylib.h"
#include <emscripten/emscripten.h>

#define MAX_PROJECTILES 100
#define MAX_ENEMIES 20

typedef struct Projectile {
    Vector2 position;
    bool active;
} Projectile;

typedef struct Enemy {
    Vector2 position;
    bool active;
} Enemy;

static Projectile projectiles[MAX_PROJECTILES] = { 0 };
static int projectileCount = 0;

static Enemy enemies[MAX_ENEMIES] = { 0 };
static int enemyCount = 0;

const int screenWidth = 800;
const int screenHeight = 450;
const float spawnCooldown = 2.5;

static int score = 0;

float lastSpawnTime;
float remainingToSpawn = spawnCooldown;

void UpdateDrawFrame(void);     // Update and Draw one frame

int main(void)
{
   for (int i = 0; i < MAX_PROJECTILES; i++) {
       projectiles[i].active = false;
       projectiles[i].position = (Vector2){0, 0};
   }

   for (int i = 0; i < MAX_ENEMIES; i++) {
       enemies[i].active = false;
       enemies[i].position = (Vector2){0, 0};
   }

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    CloseWindow();        // Close window and OpenGL context
    return 0;
}

Vector2 playerPosition = { 190, 430 };

bool aabb(Rectangle from, Rectangle to) {
    return (from.x < to.x + to.width && from.x + from.width > to.x) &&
           (from.y < to.y + to.height && from.y + from.height > to.y);
}

void UpdateDrawFrame(void)
{
    Vector2 mousePosition = GetMousePosition();
    playerPosition.x = mousePosition.x;

    if (playerPosition.x > screenWidth - 20) {
        playerPosition.x = screenWidth - 20;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 initialPosition = { playerPosition.x + 5, 420 };
        projectiles[projectileCount].position = initialPosition;
        projectiles[projectileCount].active = true;
        projectileCount++;

        if (projectileCount >= MAX_PROJECTILES) projectileCount = 0;
    }

    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            projectiles[i].position.y -= 5;
        }

        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (enemies[j].active && aabb((Rectangle){ projectiles[i].position.x, projectiles[i].position.y, 10, 10}, (Rectangle){ enemies[j].position.x, enemies[j].position.y, 20, 20})) {
               enemies[j].active = false;
               score++;
            }
        }
    }

    remainingToSpawn -= GetFrameTime();

    if (remainingToSpawn < 0) {
        remainingToSpawn = spawnCooldown;
        Vector2 initialPosition = { GetRandomValue(20, 780), 0 };
        enemies[enemyCount].active = true;
        enemies[enemyCount].position = initialPosition;
        enemyCount++;
    }

    for (int i = 0; i < MAX_ENEMIES ; i++) {
        if (enemies[i].active) {
            enemies[i].position.y += 0.8;
        }
    }


    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(playerPosition.x, playerPosition.y, 20, 20, LIGHTGRAY);
        for (int i = 0; i < MAX_PROJECTILES; i++) {
            if (projectiles[i].active) {
                DrawRectangle(projectiles[i].position.x, projectiles[i].position.y, 10, 10, LIGHTGRAY  );

                if (projectiles[i].position.y < -10) projectiles[i].active = false;
            }
        }

        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active) {
                DrawRectangle(enemies[i].position.x, enemies[i].position.y, 20, 20, RED);

                if (enemies[i].position.y > 470) enemies[i].active = false;
            }
        }
        DrawText(TextFormat("Score: %d", score), 32, 32, 20, LIGHTGRAY);
    EndDrawing();
}
