#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>
#include <chipmunk.h>

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
    
    unsigned playerEntity;

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
unsigned createPlayer(World *world);
void MovementSystem(World *world);
void RenderSystem(World *ptr);

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Simple Platform - Raylib");
    SetTargetFPS(60);

    // Initializing structures
    World world = {false,false,true,0,};
    world.playerEntity = createPlayer(&world);
    // ----------------------------------------------------


    // Main loop
    printf("Entering Main Loop");

    while (!WindowShouldClose()) {

        // Update
        InputHandler(&world);
        //----------------------------------------------

        MovementSystem(&world);

        // Rendering
        BeginDrawing();
        // Draw player
        RenderSystem(&world);

        ClearBackground(RAYWHITE);
        EndDrawing();
        // ---------------------------------------------
    }

    CloseWindow();
    return 0;
}

void RenderSystem(World *world) {
    for (unsigned entity = 0; entity < ENTITY_COUNT; entity++) {
        if ((world->mask[entity] & RENDER_MASK) == RENDER_MASK) {
            RenderableComponent renderable = world->renderComponent[entity];
            Vector2 position = renderable.renderPosition;
            Vector2 size = renderable.renderSize;
            DrawRectangle(position.x, position.y, size.x, size.y, BLACK);
        }
    }
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
    world->renderComponent[entity].renderSize.x = 50;
    world->renderComponent[entity].renderSize.y = 50;
    return entity;
}

void destroyEntity(World *world, unsigned entity) {
    world->mask[entity] = COMPONENT_NONE;
}

void MovementSystem(World *world) {
    unsigned entity;
    PositionComponent *position;
    VelocityComponent *velocity;
    RenderableComponent  *renderable;
    for (entity = 0; entity < ENTITY_COUNT; entity++) {
        if ((world->mask[entity] & MOVEMENT_MASK) == MOVEMENT_MASK) {
            position = &world->positionComponent[entity];
            velocity = &world->velocityComponent[entity];
            renderable = &world->renderComponent[entity];

            position->x += velocity->x;
            position->y += velocity->y;
            renderable->renderPosition.x += velocity->x;
            renderable->renderPosition.y += velocity->y;
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

        VelocityComponent *playerVelocity = &world->velocityComponent[world->playerEntity];
        if (IsKeyDown(KEY_DOWN)) {
        } else if (IsKeyDown(KEY_UP)) {
        } else if (IsKeyDown(KEY_LEFT)) {
            playerVelocity->x = -5;
        } else if (IsKeyDown(KEY_RIGHT)) {
            playerVelocity->x = 5;
        } else {
            if (playerVelocity->x > 0) {
                playerVelocity->x -= 1;
            }
            if (playerVelocity->x < 0) {
                playerVelocity->x += 1;
            }
        }
    }
}