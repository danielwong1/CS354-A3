
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
class Arrow;
class Goal;

// Start this at 16 so we don't get 0 by accident
enum MessageType { KICK = (1 << 4), BALL_POS };

struct KickMessage {
    float x_val;
    float y_val;
    float z_val;
};

struct BallPositionMessage {
    float x_coord;
    float y_coord;
    float z_coord;
};

class BallGame : public BaseApplication
{
public:
    bool kicked;
    static std::string ballString;
    static std::string botString;
    static int rotationBound;
    Physics* simulator;
    Score* scoreObj;
    btClock* collisionClock;
    btClock* f_collisionClock;
    bool started;
    Ogre::Vector2 mRot;
    std::string* ipAddr;
    bool isHost;
    NetManager* network;

    BallGame(void);
    virtual ~BallGame(void);
    virtual void go();

    void createCollisionCallbacks(void);
    void addResources(void);
    void destroyArrow(void);
	Paddle* mPaddle;
	Ball* mBall;
    Arrow* mArrow;
    Goal* mGoal;
	Field* mField;

protected:
	BallScoreCallback* mBallScoreCallback;
	BallPaddleCallback* mBallPaddleCallback;
    BallFloorCallback* mBallFloorCallback;
    WallCallback* mWallCallback;
    CEGUI::OgreRenderer* mRenderer;
    CEGUI::Window* startRoot;

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual void createScene(void);
    virtual bool mouseMoved(const OIS::MouseEvent &ev);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool keyPressed( const OIS::KeyEvent &arg);
    void setupCEGUI(void);

    void reset(btTransform ballTransform, btVector3 origin);

    void setupSDL(void);
    void setupNetwork(void);
    bool hostClick(const CEGUI::EventArgs &e);
    bool clientClick(const CEGUI::EventArgs &e);
private:
    int currentRotationX;
    int currentRotationY;

};

#endif 
