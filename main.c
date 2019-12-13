#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>

typedef struct {
    bool mainMenu;
    bool isPaused;
    bool isPlaying;
    int menuOption;
} World;

typedef struct {
    Vector2 position;
    int size;
    float speed;
} Player;

void InputHandler(World *world, Player *player);

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Simple Platform - Raylib");

    SetTargetFPS(60);

    // Initializing structures

    // World State structure
    World world = {
            false,
            false,
            true,
            0,
    };

    Player player = {
            .position = {
                    .x = 50,
                    .y = 50
            },
            .speed = 10,
            .size = 50
    };
    // ----------------------------------------------------


    // Main loop
    printf("Entering Main Loop");
    while (!WindowShouldClose()) {

        // Update
        InputHandler(&world, &player);
        //----------------------------------------------

        // Rendering
        BeginDrawing();

        // Draw player
        DrawRectangle(player.position.x, player.position.y, player.size, player.size, SKYBLUE);

        ClearBackground(RAYWHITE);
        EndDrawing();
        // ---------------------------------------------
    }

    CloseWindow();
    return 0;
}

void InputHandler(World *world, Player *player) {
    if (world->mainMenu) {
        if (IsKeyDown(KEY_DOWN)) {
            if (world->menuOption != 0) {
                world->menuOption++;
                printf("%d\n", world->menuOption);
            }
        } else if (IsKeyDown(KEY_UP)) {
            if (world->menuOption < 5) {
                world->menuOption--;
                printf("%d\n", world->menuOption);
            }
        }
    }
    if (world->isPlaying) {
        if (IsKeyDown(KEY_DOWN)) {
            player->position.y += player->speed;
        } else if (IsKeyDown(KEY_UP)) {
            player->position.y -= player->speed;
        } else if (IsKeyDown(KEY_LEFT)) {
            player->position.x -= player->speed;
        } else if (IsKeyDown(KEY_RIGHT)) {
            player->position.x += player->speed;
        }
    }
}