#ifndef WORLD_H
#define WORLD_H

#include "components.h"

#define ENTITY_COUNT 100

typedef struct World {
    bool mainMenu;
    bool isPaused;
    bool isPlaying;
    int menuOption;
    
    unsigned playerEntity;
    Camera2D camera;

    cpSpace *space;
    cpFloat timeStep;

    int mask[ENTITY_COUNT];
    PositionComponent positionComponent[ENTITY_COUNT];
    VelocityComponent velocityComponent[ENTITY_COUNT];
    RenderableComponent renderComponent[ENTITY_COUNT];
    PhysicsComponent physicsComponent[ENTITY_COUNT];
} World;

#endif