#ifndef __Wall_h_
#define __Wall_h_

#include "GameObject.h"

class Wall : public GameObject {
    public:
    
    static const int GRID_SIZE = 20;
    Wall(Ogre::String name, 
    Ogre::SceneManager* sceneMgr, 
    Physics* simulator,
    Ogre::Vector3 direction);
};

#endif