
#ifndef __BallGame_h_
#define __BallGame_h_

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include <OgreRoot.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>
#include <string>
#include "Physics.h"
#include "BaseApplication.h"
#include "Score.h"
#include "Field.h"
#include "NetManager.h"

class Paddle;
class Ball;
class Wall;
class BallScoreCallback;
class BallPaddleCallback;
class BallFloorCallback;
class WallCallback;

class BallGame : public BaseApplication
{
public:
    static std::string ballString;
    static std::string botString;
    static int rotationBound;
    Physics* simulator;
    Score* scoreObj;
    btClock* collisionClock;
    btClock* f_collisionClock;
    bool started;
    bool isHost;
    Ogre::Vector2 mRot;
    NetManager* network;
    std::string* ipAddr;

    BallGame(void);
    virtual ~BallGame(void);
    virtual void go();

    void createCollisionCallbacks(void);
    void addResources(void);

	Paddle* mPaddle;
	Ball* mBall;

	Field* mField;

protected:
	BallScoreCallback* mBallScoreCallback;
	BallPaddleCallback* mBallPaddleCallback;
    BallFloorCallback* mBallFloorCallback;
    WallCallback* mWallCallback;
    CEGUI::OgreRenderer* mRenderer;
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual void createScene(void);
    virtual bool mouseMoved(const OIS::MouseEvent &ev);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual void createViewports(void);
    void setupCEGUI(void);

    void reset(btTransform ballTransform, btVector3 origin);

    void setupSDL(void);
    void setupNetwork(void);
    bool hostClick(const CEGUI::EventArgs &e);

private:
    int currentRotationX;
    int currentRotationY;

};

#endif 
