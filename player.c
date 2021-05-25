#include "player.h"
#include <raylib.h>
#include "vendor/Chipmunk2D/include/chipmunk/chipmunk.h"
#include "components.h"
#include "world.h"
#include "masks.h"

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
    cpBodySetPosition(body, cpv(10, 20));

    cpShape *shape = cpSpaceAddShape(space, cpBoxShapeNew(body, 10, 10, 0));
    cpShapeSetFriction(shape, 0.1);

    world->physicsComponent[entity].body = body;
    world->physicsComponent[entity].shape = shape;

    return entity;
}