#ifndef __GoalieWall_h_
#define __GoalieWall_h_

#include "GameObject.h"

class GoalieWall : public GameObject {
    public:
        GoalieWall(Ogre::String name, Ogre::SceneManager* sceneMgr,
    Physics* simulator);

};

#endif