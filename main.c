#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include "vendor/Chipmunk2D/include/chipmunk/chipmunk.h"
#include "components.h"
#include "world.h"
#include "player.h"
#include "masks.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 650

typedef struct {
    Vector2 position;
    int size;
    float speed;
} Player;

void ControllerSystem(World *world);
unsigned CreateEntity(World *world);
void DestroyEntity(World *world, unsigned entity);
void CreateSpace(World *world);
void DestroyEntities(World *world);
void MovementSystem(World *world);
void RenderSystem(World *world);
void CameraSystem(World *world);
void TransformationMatrixSystem(World *world);
void PhysicsSystem(World *world);

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple Platform - Raylib");
    SetTargetFPS(60);

    // Initializing structures
    World world = {false,false,true,0,};
    CreateSpace(&world);
    world.playerEntity = CreatePlayer(&world);
    world.camera.zoom = 1.0f;
    world.camera.rotation = 180.0f;

    // Main loop
    printf("Entering Main Loop");

    world.timeStep = 1.0 / 60.0;
    while (!WindowShouldClose()) {
        ControllerSystem(&world);
        PhysicsSystem(&world);
        RenderSystem(&world);
    }
    DestroyEntities(&world);
    CloseWindow();
    return 0;
}

void CreateSpace(World *world) {
    world->space = cpSpaceNew();

    cpVect gravity = cpv(0, -100);
    cpSpaceSetGravity(world->space, gravity);

    // unsigned int groundEntity = CreateEntity(world);
    cpShape *shape;
    cpSpace *space = world->space;
    cpBody *staticBody = cpSpaceGetStaticBody(space);

    // Create segments around edge of escreen
    cpVect p1 = cpv(0, 0);
    cpVect p2 = cpv(0, 600);
    cpVect p3 = cpv(1200, 600);
    cpVect p4 = cpv(1200, 0);

    cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, p1, p2, 1.0f));
	cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, p2, p3, 1.0f));
	cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, p3, p4, 1.0f));
	cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, p4, p1, 1.0f));

    // world->mask[groundEntity] = (COMPONENT_PHYSICS | COMPONENT_RENDER);
    // world->physicsComponent[groundEntity].shape = shape;
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
            playerVelocity->x = -100;
        } else if (IsKeyDown(KEY_RIGHT)) {
            playerVelocity->x = 100;
        } else {
            playerVelocity->x = 0;
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
                printf("Entity[%d] Position: X: %f Y: %f\n", i, position.x, position.y);

                VelocityComponent velocity = world->velocityComponent[i];
                float delta = GetFrameTime();
                cpBodySetForce(component.body, cpv(velocity.x, velocity.y));
            }
        }
    }
}

void CameraSystem(World *world) {
    static float minSpeed = 50;
    static float minEffectLength = 10;
    static float fractionSpeed = 0.8f;

    world->camera.offset = (Vector2){ SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f };
    cpVect bodyPosition = cpBodyGetPosition(world->physicsComponent[world->playerEntity].body);
    Vector2 diff = Vector2Subtract((Vector2){bodyPosition.x, bodyPosition.y}, world->camera.target);
    float length = Vector2Length(diff);

    if (length > minEffectLength) {
        float speed = fmaxf(fractionSpeed*length, minSpeed);
        world->camera.target = Vector2Add(world->camera.target, Vector2Scale(diff, speed*GetFrameTime()/length));
    }
}

void RenderSystem(World *world) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    rlPushMatrix();
    rlTranslatef(0, GetScreenHeight(), 0);
    rlScalef(1, -1, 1);
    rlDisableBackfaceCulling();
    for (unsigned entity = 0; entity < ENTITY_COUNT; entity++) {
        if ((world->mask[entity] & (RENDER_MASK | COMPONENT_PHYSICS)) == (RENDER_MASK | COMPONENT_PHYSICS)) {
            Vector2 position = world->renderComponent[entity].renderPosition;
            Vector2 size = world->renderComponent[entity].renderSize;

            cpVect bodyPosition = cpBodyGetPosition(world->physicsComponent[entity].body);
            DrawRectangle(bodyPosition.x, bodyPosition.y, size.x, size.y, BLACK);
            printf("Drawing[%d] Rect: X: %f Y: %f sX: %f sY: %f\n", entity, bodyPosition.x, bodyPosition.y, size.x, size.y);
        }
    }
    rlDrawRenderBatchActive();
    rlEnableBackfaceCulling();
    rlPopMatrix();
    EndDrawing();
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