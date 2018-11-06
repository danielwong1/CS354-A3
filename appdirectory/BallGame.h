
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
#include "Wall.h"
#include "GoalieWall.h"
#include "NetManager.h"

class Paddle;
class Ball;
class Wall;
class BallScoreCallback;
class BallBoundaryCallback;
class Arrow;
class Goal;

// Start this at 16 so we don't get 0 by accident
enum MessageType { KICK = (1 << 4), POSITION, SCORE };

struct KickMessage {
    float x_val;
    float y_val;
    float z_val;
};

struct PositionMessage {
    float ballXCoord;
    float ballYCoord;
    float ballZCoord;

    float paddleXCoord;
    float paddleYCoord;
    float paddleZCoord;
};

struct ScoreMessage {
    int player;
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
    int playerScore;
    int hostScore;
    btClock* collisionClock;
    btClock* f_collisionClock;
    Ogre::Vector2 mRot;
    std::string ipAddr;
    bool isHost;
    bool started;
    NetManager* network;

    BallGame(void);
    virtual ~BallGame(void);
    virtual void go();
    void createViewports();

    void createDefaultCamera(void);
    void createCollisionCallbacks(void);
    void addResources(void);
    void destroyArrow(void);
    void createGame(void);
    void setupScore(void);
	Paddle* mPaddle;
	Ball* mBall;
    Arrow* mArrow;
    Goal* mGoal;
	Field* mField;
    Wall* lWall;
    Wall* rWall;
    Wall* tWall;
    Wall* cWall;
    Wall* hWall;
    GoalieWall* gWall;
    CEGUI::Window* scoreRoot;
    void reset();

protected:
	BallScoreCallback* mBallScoreCallback;
    BallBoundaryCallback* mBallBoundaryCallback;
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

    

    void setupSDL(void);
    void setupNetwork(void);
    bool hostClick(const CEGUI::EventArgs &e);
    bool clientClick(const CEGUI::EventArgs &e);
private:
    int currentRotationX;
    int currentRotationY;

};

#endif 
