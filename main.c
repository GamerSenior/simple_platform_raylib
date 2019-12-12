#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>

typedef struct {
    bool mainMenu;
    bool isPaused;
    int menuOption;
} World;

void InputHandler(World *world);

int main() {
    // World State structure
    World world = {
            true,
            false,
            0,
    };

    SetTargetFPS(60);
    // Main loop
    while (!WindowShouldClose()) {
        InputHandler(&world);
    }
}

void InputHandler(World *world) {
    if (world->mainMenu) {
        if (IsKeyDown(KEY_DOWN)) {
            if (world->menuOption != 0) {
                world->menuOption++;
                printf("%d", world->menuOption);
            }
        } else if (IsKeyDown(KEY_UP)) {
            if (world->menuOption < 5) {
                world->menuOption--;
                printf("%d", world->menuOption);
            }
        }
    }
}