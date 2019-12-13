#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>
#define ENTITY_COUNT 100

typedef Vector2 PositionComponent;
typedef Vector2 VelocityComponent;
typedef struct {
    Vector2 renderPosition;
    Vector2 renderSize;
} RenderableComponent;

typedef enum {
    COMPONENT_NONE = 0,
    COMPONENT_POSITION = 1 << 0,
    COMPONENT_VELOCITY = 2 << 1,
    COMPONENT_RENDER = 3 << 2,
    COMPONENT_PLAYER = 4 << 3,
} Component;

#define MOVEMENT_MASK (COMPONENT_POSITION | COMPONENT_VELOCITY)
#define RENDER_MASK (COMPONENT_RENDER)
#define PLAYER_MASK (COMPONENT_RENDER | COMPONENT_POSITION | COMPONENT_VELOCITY)

typedef struct {
    bool mainMenu;
    bool isPaused;
    bool isPlaying;
    int menuOption;

    int mask[ENTITY_COUNT];
    PositionComponent positionComponent[ENTITY_COUNT];
    VelocityComponent velocityComponent[ENTITY_COUNT];
    RenderableComponent renderComponent[ENTITY_COUNT];
} World;

typedef struct {
    Vector2 position;
    int size;
    float speed;
} Player;

void InputHandler(World *world);
unsigned createEntity(World *world);
void destroyEntity(World *world, unsigned entity);
void movementSystem(World *world);
unsigned createPlayer(World *world);

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Simple Platform - Raylib");
    SetTargetFPS(60);

    // Initializing structures
    World world = {false,false,true,0,};
    unsigned playerEntity = createPlayer(&world);
    // ----------------------------------------------------


    // Main loop
    printf("Entering Main Loop");
    while (!WindowShouldClose()) {

        // Update
        InputHandler(&world);
        //----------------------------------------------

        // Rendering
        BeginDrawing();

        // Draw player

        ClearBackground(RAYWHITE);
        EndDrawing();
        // ---------------------------------------------
    }

    CloseWindow();
    return 0;
}

unsigned createEntity(World *world) {
    unsigned entity;
    for (entity = 0; entity < ENTITY_COUNT; entity++) {
        if (world->mask[entity] == COMPONENT_NONE) {
            return entity;
        }
    }
    printf("Error: No more entities left");
    return ENTITY_COUNT;
}

unsigned createPlayer(World *world) {
    unsigned entity = createEntity(world);
    world->mask[entity] = PLAYER_MASK;
    world->positionComponent[entity].x = 50;
    world->positionComponent[entity].y = 50;
    world->velocityComponent[entity].x = 0;
    world->velocityComponent[entity].y = 0;
    world->renderComponent[entity].renderPosition.x = 50;
    world->renderComponent[entity].renderPosition.y = 50;
    return entity;
}

void destroyEntity(World *world, unsigned entity) {
    world->mask[entity] = COMPONENT_NONE;
}

void movementSystem(World *world) {
    unsigned entity;
    PositionComponent *position;
    VelocityComponent *velocity;
    for (entity = 0; entity < ENTITY_COUNT; entity++) {
        if ((world->mask[entity] & MOVEMENT_MASK) == MOVEMENT_MASK) {
            position = &world->positionComponent[entity];
            velocity = &world->velocityComponent[entity];

            position->x += velocity->x;
            position->y += velocity->y;
        }
    }
}

void InputHandler(World *world) {
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
        /* TODO Think about how to handle PLAYER movement
         * maybe keep the player entity globally stored?
        */
        if (IsKeyDown(KEY_DOWN)) {
        } else if (IsKeyDown(KEY_UP)) {
        } else if (IsKeyDown(KEY_LEFT)) {
        } else if (IsKeyDown(KEY_RIGHT)) {
        }
    }
}