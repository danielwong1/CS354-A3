#ifndef __Field_h_
#define __Field_h_

#include "GameObject.h"

class Field : public GameObject {
 public:
  static const int SIZE = 10;

  Field(
    Ogre::SceneManager* sceneMgr, 
    Physics* simulator
  );
};

#endif