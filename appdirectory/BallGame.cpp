#include "BallGame.h"
#include <SDL2/SDL.h>
#include "src/audio.h"
#include <OgreConfigFile.h>
#include "OgreRenderWindow.h"
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include "OgreViewport.h"
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <OgreVector3.h>
#include <OgreMeshManager.h>
#include <OgreResourceGroupManager.h>
#include <time.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "Ball.h"
#include "Paddle.h"
#include "BallScoreCallback.h"
#include "BallPaddleCallback.h"
#include "BallFloorCallback.h"
#include "WallCallback.h"
#include "Field.h"
#include "Arrow.h"
#include "Goal.h"

std::string BallGame::ballString = "ball";
std::string BallGame::botString = "bot";
int BallGame::rotationBound = 400;

BallGame::BallGame() : mRenderer(0)
{
    currentRotationX = 0;
    currentRotationY = 0;
}

BallGame::~BallGame(void)
{
    CEGUI::OgreRenderer::destroySystem();
}

void BallGame::reset(btTransform ballTransform, btVector3 origin) {
    started = false;
    /*ballTransform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
    ballTransform.setRotation(btQuaternion::getIdentity());

    mBall->motionState->setWorldTransform(ballTransform);
    mBall->body->setWorldTransform(ballTransform);
    mBall->body->clearForces();
    mBall->body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
    mBall->body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));

    mBall->floorBounces = 0;
    mBall->body->applyCentralImpulse(btVector3(0.0f, 0.0f, -1.0f));
    if(soundOn) { 
        playSound("sounds/ball_miss.wav", SDL_MIX_MAXVOLUME);
    }
    scoreObj->setScore(0);*/
}

bool BallGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;
 
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    double cameraSpeed = 0.005;
    double kickForce = 3.0f;
    Ogre::Radian rotationSpeed = Ogre::Radian(Ogre::Degree(.1));
    Ogre::Vector3 cameraPos = mCamera->getPosition();

    if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        return false;


    if(mKeyboard->isKeyDown(OIS::KC_UP)) {
            currentRotationY++;
            mArrow->rotateArrowBy(Ogre::Quaternion(rotationSpeed, Ogre::Vector3(1, 0, 0)));
    }

    if(mKeyboard->isKeyDown(OIS::KC_RIGHT)) {
            currentRotationX++;
            mArrow->rotateArrowBy(Ogre::Quaternion(-rotationSpeed, Ogre::Vector3(0, 1, 0)));
    }

    if(mKeyboard->isKeyDown(OIS::KC_DOWN)) {
            currentRotationY--;
            mArrow->rotateArrowBy(Ogre::Quaternion(-rotationSpeed, Ogre::Vector3(1, 0, 0)));
    }

    if(mKeyboard->isKeyDown(OIS::KC_LEFT)) {
            currentRotationX--;
            mArrow->rotateArrowBy(Ogre::Quaternion(rotationSpeed, Ogre::Vector3(0, 1, 0)));
    }

    btTransform trans;
    mBall->motionState->getWorldTransform(trans);
    printf("%f %f\n", trans.getOrigin().y(), mBall->rootNode->getPosition().y);
    if(mKeyboard->isKeyDown(OIS::KC_SPACE) && !started) {
        started = true;
        Ogre::Vector3 direction = mArrow->rootNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
        direction.normalise();
        mBall->body->applyCentralImpulse(kickForce * btVector3(direction.x, direction.y, direction.z));
        mBall->body->setActivationState(ACTIVE_TAG);
    }

    cameraPos = mCamera->getPosition();
    mCamera->setPosition(cameraPos + Ogre::Vector3(cameraSpeed * mRot.x, -cameraSpeed * mRot.y, 0));
    mRot = Ogre::Vector2::ZERO;
    
    if(simulator != NULL) {
        simulator->dynamicsWorld->stepSimulation(simulator->physicsClock->getTimeSeconds());
        simulator->physicsClock->reset();
    }
    
    return true;
}

void BallGame::setupCEGUI(void) {
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
    /*CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    startRoot = wmgr.loadLayoutFromFile( "menu.layout" );
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow( startRoot );
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");
    CEGUI::FontManager::getSingleton().createFreeTypeFont( "DejaVuSans-12", 12, true, "DejaVuSans.ttf", "Fonts");
    CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont( "DejaVuSans-12" );

    startRoot->getChild("Host")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&BallGame::hostClick, this));
    startRoot->getChild("Kicker")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&BallGame::hostClick, this));*/
}

void BallGame::addResources() {
    Ogre::ResourceGroupManager* resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
    resourceGroupManager->addResourceLocation(
        "./models",
        "FileSystem",
        "Assignment3/Models"
    );
    resourceGroupManager->addResourceLocation(
        "./materials",
        "FileSystem",
        "Assignment3/Materials"
    );
    resourceGroupManager->addResourceLocation(
        "./textures",
        "FileSystem",
        "Assignment3/Textures"
    );

    resourceGroupManager->initialiseResourceGroup("Assignment3/Materials");
    resourceGroupManager->initialiseResourceGroup("Assignment3/Models");
    resourceGroupManager->initialiseResourceGroup("Assignment3/Textures");
}

void BallGame::setupSDL()
{
    /* Initialize only SDL Audio on default device */
    if(SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        return;
    }

    initAudio();

    /* Play music and a sound */
    playMusic("music/highlands.wav", SDL_MIX_MAXVOLUME);
    //playSound("sounds/door1.wav", SDL_MIX_MAXVOLUME / 2);
}

void BallGame::createScene(void)
{
    setupCEGUI();
    setupSDL();
    addResources();
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

    mField = new Field(mSceneMgr, simulator);
    mBall = new Ball(ballString, mSceneMgr, simulator);
    //mBall->moveTo(Ogre::Vector3(0.0, 0.0, -Field::SIZE / 2 - 10));
    mArrow = new Arrow(mSceneMgr);
    mArrow->moveArrowTo(Ogre::Vector3(-5.0, 5.0, 10.0 ));
    mBall->moveTo(Ogre::Vector3(0.0, 4.0, 10.0));
    //mPaddle = new Paddle(mSceneMgr, simulator);

    mSceneMgr->setSkyBox(true, "sky/Material");
    mGoal = new Goal(mSceneMgr, simulator);
    mGoal->moveBy(Ogre::Vector3(0, 4.5, -25));
    mGoal->rotateBy(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(-90)), Ogre::Vector3::UNIT_Y));

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    Ogre::Light* light = mSceneMgr->createLight("SpotLight");
    light->setPosition(Ogre::Vector3(0, 10, 0));
    mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    light->setCastShadows(true);

    createCollisionCallbacks();
}

void BallGame::createCollisionCallbacks(void) {
    // mBallScoreCallback = new BallScoreCallback(this);
    // mBallFloorCallback = new BallFloorCallback(this);
    mBallPaddleCallback = new BallPaddleCallback(this);
    // mWallCallback = new WallCallback(this);
}

bool BallGame::mouseMoved(const OIS::MouseEvent &ev) {
    mRot.x = ev.state.X.rel;
    mRot.y = ev.state.Y.rel;
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(
                ev.state.X.rel, 
                ev.state.Y.rel);
}

void BallGame::destroyArrow(void) {
    Ogre::SceneNode* parent = mArrow->rootNode->getParentSceneNode();
    parent->detachObject(mArrow->name);
    mSceneMgr->destroyEntity(mArrow->geom->getName());
}


bool BallGame::hostClick(const CEGUI::EventArgs &e) {
    startRoot->hide();
    scoreObj = new Score(); 
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide( );
    return true;
}

bool BallGame::clientClick(const CEGUI::EventArgs &e) {
    startRoot->hide(); 
    scoreObj = new Score();
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide( );
    return true;
}

bool BallGame::mouseReleased(const OIS::MouseEvent &ev, OIS::MouseButtonID id) {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton);
}

bool BallGame::mousePressed(const OIS::MouseEvent &ev, OIS::MouseButtonID id) {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton);
}

bool BallGame::keyPressed( const OIS::KeyEvent &arg )
{
    if (arg.key == OIS::KC_ESCAPE) {
      mShutDown = true;
    }

    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan)arg.key);
    CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(arg.text);

    return true;
}
//---------------------------------------------------------------------------
bool BallGame::keyReleased(const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_O) {
        soundOn = !soundOn;
    }
    if (arg.key == OIS::KC_M) {
        if(musicOn) {
            pauseAudio();
            musicOn = false;
        } else {
            unpauseAudio();
            musicOn = true;
        }
    }

    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
    return true;
}

void BallGame::go()
{
    soundOn = true;
    started = false;
    simulator = new Physics();
    collisionClock = new btClock();
    f_collisionClock = new btClock();
    BaseApplication::go();
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT) {

#else
    int main(int argc, char *argv[]) {
#endif
        
        // Create application object
        BallGame app;

        const struct option long_options[] = {
            {"ip", 1, 0, 'i'},
            {0,0,0,0},
        };
        int option_index = 0;
        int c = 0;

        while(c != -1) {
            c = getopt_long(argc, argv, "i:", long_options, &option_index);
            switch(c) 
            {
                case 'i':
                    app.ipAddr = new std::string(optarg);
                    printf("ip: %s", app.ipAddr->c_str());
                    break;
            }
        }

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
