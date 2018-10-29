#ifndef __Goal_h_
#define __Goal_h_

#include "GameObject.h"

class Goal : public GameObject {
    public:
    Goal(
    Ogre::SceneManager* sceneMgr, 
    Physics* simulator);
};

#endif