#ifndef __PHYSICS_H_
#define __PHYSICS_H_

#include <vector>
#include "GameObject.h"
#include <btBulletDynamicsCommon.h>

enum collisionMask {
    GOAL = 1,
    GOAL_WALL = 2,
    ETC = 4
};

class Physics {
    public: 
    btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<GameObject *> gameObjects;
    
    btClock* physicsClock;
    Physics();
};

#endif