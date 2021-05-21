#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>
#include "vendor/Chipmunk2D/include/chipmunk/chipmunk.h"
#include "components.h"

#define ENTITY_COUNT 100

#define MOVEMENT_MASK (COMPONENT_POSITION | COMPONENT_VELOCITY)
#define RENDER_MASK (COMPONENT_RENDER)
#define PLAYER_MASK (COMPONENT_RENDER | COMPONENT_POSITION | COMPONENT_VELOCITY | COMPONENT_PHYSICS)

typedef struct {
    bool mainMenu;
    bool isPaused;
    bool isPlaying;
    int menuOption;
    
    unsigned playerEntity;

    cpSpace *space;
    cpFloat timeStep;

    int mask[ENTITY_COUNT];
    PositionComponent positionComponent[ENTITY_COUNT];
    VelocityComponent velocityComponent[ENTITY_COUNT];
    RenderableComponent renderComponent[ENTITY_COUNT];
    PhysicsComponent physicsComponent[ENTITY_COUNT];
} World;

typedef struct {
    Vector2 position;
    int size;
    float speed;
} Player;

void ControllerSystem(World *world);
unsigned CreateEntity(World *world);
void DestroyEntity(World *world, unsigned entity);
unsigned CreatePlayer(World *world);
void CreateSpace(World *world);
void DestroyEntities(World *world);
void MovementSystem(World *world);
void RenderSystem(World *ptr);
void PhysicsSystem(World *world);

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Simple Platform - Raylib");
    SetTargetFPS(60);

    // Initializing structures
    World world = {false,false,true,0,};
    CreateSpace(&world);
    world.playerEntity = CreatePlayer(&world);
    // ----------------------------------------------------


    // Main loop
    printf("Entering Main Loop");

    world.timeStep = 1.0 / 60.0;
    while (!WindowShouldClose()) {

        // Input handler
        ControllerSystem(&world);
        //----------------------------------------------

        // Rendering
        BeginDrawing();
        
        PhysicsSystem(&world);
        // RenderSystem(&world);

        ClearBackground(RAYWHITE);
        EndDrawing();
        // ---------------------------------------------
    }

    DestroyEntities(&world);

    CloseWindow();
    return 0;
}

void CreateSpace(World *world) {
    world->space = cpSpaceNew();

    cpVect gravity = cpv(0, -100);
    cpSpaceSetGravity(world->space, gravity);

    // Create simple line to test gravity and colision
    unsigned int groundEntity = CreateEntity(world);
    cpShape *ground = cpSegmentShapeNew(cpSpaceGetStaticBody(world->space), cpv(0, 0), cpv(100, 0), 0);
    cpShapeSetFriction(ground, 1);
    cpSpaceAddShape(world->space, ground);
    world->mask[groundEntity] = (COMPONENT_PHYSICS | COMPONENT_RENDER);
    world->physicsComponent[groundEntity].shape = ground;
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

void PhysicsSystem(World *world) {
    cpSpaceStep(world->space, world->timeStep);

    for (unsigned i = 0; i < ENTITY_COUNT; i++) {
        if ((world->mask[i] & COMPONENT_PHYSICS) == COMPONENT_PHYSICS) {
            PhysicsComponent component = world->physicsComponent[i];
            if (component.body) {
                cpVect position = cpBodyGetPosition(component.body);

                VelocityComponent velocity = world->velocityComponent[i];
                cpBodySetForce(component.body, cpv(velocity.x, velocity.y));

                printf("Entity[%d] Position: X: %f Y: %f\n", i, position.x, position.y);
            }
        }
    }
}

void ControllerSystem(World *world) {
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
        VelocityComponent *playerVelocity = &(world->velocityComponent[world->playerEntity]);
        if (IsKeyDown(KEY_DOWN)) {
        } else if (IsKeyDown(KEY_UP)) {
        } else if (IsKeyDown(KEY_LEFT)) {
            playerVelocity->x = -60;
        } else if (IsKeyDown(KEY_RIGHT)) {
            playerVelocity->x = 60;
        } else {
            playerVelocity->x = 0;
        }
    }
}

unsigned CreateEntity(World *world) {
    unsigned entity;
    for (entity = 0; entity < ENTITY_COUNT; entity++) {
        if (world->mask[entity] == COMPONENT_NONE) {
            return entity;
        }
    }
    printf("Error: No more entities left");
    return ENTITY_COUNT;
}

unsigned CreatePlayer(World *world) {
    unsigned entity = CreateEntity(world);
    world->mask[entity] = PLAYER_MASK;
    world->positionComponent[entity].x = 10;
    world->positionComponent[entity].y = 10;
    world->velocityComponent[entity].x = 0;
    world->velocityComponent[entity].y = 0;
    world->renderComponent[entity].renderPosition.x = 50;
    world->renderComponent[entity].renderPosition.y = 50;
    world->renderComponent[entity].renderSize.x = 50;
    world->renderComponent[entity].renderSize.y = 50;

    // Create body and shape for Physics
    cpSpace *space = world->space;
    cpFloat mass = 1;
    cpFloat moment = cpMomentForBox(mass, 1, 1);
    cpBody *body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPosition(body, cpv(10, 10));

    cpShape *shape = cpSpaceAddShape(space, cpBoxShapeNew(body, 1, 1, 0));
    cpShapeSetFriction(shape, 0.3);

    world->physicsComponent[entity].body = body;
    world->physicsComponent[entity].shape = shape;

    return entity;
}

void DestroyEntity(World *world, unsigned entity) {
    if ((world->mask[entity] & COMPONENT_PHYSICS) == COMPONENT_PHYSICS) {
        PhysicsComponent component = world->physicsComponent[entity];
        cpShapeFree(component.shape);
        cpBodyFree(component.body);
    }
    world->mask[entity] = COMPONENT_NONE;
}

void DestroyEntities(World *world) {
    for (unsigned i = 0; i < ENTITY_COUNT; i++) {
        DestroyEntity(world, i);
    }
}