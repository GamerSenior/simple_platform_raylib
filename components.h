#include <raylib.h>                                         // Required for Vector2
#include "vendor/Chipmunk2D/include/chipmunk/chipmunk.h"    // Required for cpShape, cpBody

#ifndef COMPONENTS_H
#define COMPONENTS_H

typedef Vector2 PositionComponent;
typedef Vector2 VelocityComponent;
typedef struct RenderableComponent {
    Vector2 renderPosition;
    Vector2 renderSize;
} RenderableComponent;

typedef struct PhysicsComponent {
    cpShape *shape;
    cpBody *body;
} PhysicsComponent;

typedef enum Component {
    COMPONENT_NONE = 0,
    COMPONENT_POSITION = 1 << 0,
    COMPONENT_VELOCITY = 2 << 1,
    COMPONENT_RENDER = 3 << 2,
    COMPONENT_PLAYER = 4 << 3,
    COMPONENT_PHYSICS = 5 << 4
} Component;

#endif