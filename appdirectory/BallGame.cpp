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
#include "Field.h"
#include "Goal.h"

std::string BallGame::ballString = "ball";
std::string BallGame::botString = "bot";
int BallGame::rotationBound = 400;

BallGame::BallGame() : mRenderer(0)
{
    ipAddr = NULL;
    currentRotationX = 0;
    currentRotationY = 0;
}

BallGame::~BallGame(void)
{
    CEGUI::OgreRenderer::destroySystem();
}

void BallGame::reset(btTransform ballTransform, btVector3 origin) {
    started = false;
    ballTransform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
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
    scoreObj->setScore(0);
}

bool BallGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;
 
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    Ogre::Camera* actingCamera = isHost ? hostCamera : mCamera;

    double cameraSpeed = 0.005;
    Ogre::Radian rotationSpeed = Ogre::Radian(Ogre::Degree(.1));
    Ogre::Vector3 cameraPos = actingCamera->getPosition();

    if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        return false;


    if(mKeyboard->isKeyDown(OIS::KC_SPACE) && !started) {
        

        if (!isHost) {
            std::string message = "1";
            const char* c_message = message.c_str();
            network->messageServer(PROTOCOL_TCP, c_message, strlen(c_message) + 1);
        }
        else {
            started = true;
        }
    }

    // actingCamera->lookAt(Ogre::Vector3(0,-Wall::GRID_SIZE/4 ,-Wall::GRID_SIZE));

    if(mKeyboard->isKeyDown(OIS::KC_UP)) {
        if (currentRotationY < rotationBound) {
            currentRotationY++;
            mPaddle->rotateBy(Ogre::Quaternion(-rotationSpeed, Ogre::Vector3(1, 0, 0)));
        }
    }

    if(mKeyboard->isKeyDown(OIS::KC_RIGHT)) {
        if (currentRotationX < rotationBound) {
            currentRotationX++;
            mPaddle->rotateBy(Ogre::Quaternion(rotationSpeed, Ogre::Vector3(0, 1, 0)));
        }
    }

    if(mKeyboard->isKeyDown(OIS::KC_DOWN)) {
        if (currentRotationY > -rotationBound) {
            currentRotationY--;
            mPaddle->rotateBy(Ogre::Quaternion(rotationSpeed, Ogre::Vector3(1, 0, 0)));
        }
    }

    if(mKeyboard->isKeyDown(OIS::KC_LEFT)) {
        if (currentRotationX > -rotationBound) {
            currentRotationX--;
            mPaddle->rotateBy(Ogre::Quaternion(-rotationSpeed, Ogre::Vector3(0, 1, 0)));
        }
    }


    cameraPos = actingCamera->getPosition();
    actingCamera->setPosition(cameraPos + Ogre::Vector3(
        cameraSpeed * mRot.x * (isHost ? -1 : 1),
        -cameraSpeed * mRot.y,
        0
    ));
    mPaddle->moveBy(Ogre::Vector3(
        cameraSpeed * mRot.x * (isHost ? -1 : 1),
        -cameraSpeed * mRot.y,
        0
    ));
    mRot = Ogre::Vector2::ZERO;
    btVector3 position = mPaddle->getPosition();
    // mPaddle->moveTo(Ogre::Vector3(
    //     // Ogre::Math::Clamp(position.x(), -(Wall::GRID_SIZE * 0.5f) + 3.0f, Wall::GRID_SIZE * 0.5f - 3.0f),
    //     // Ogre::Math::Clamp(position.y(), -(Wall::GRID_SIZE * 0.5f) + 3.0f, Wall::GRID_SIZE * 0.5f - 3.0f),
    //     position.z()
    // ));
    // cameraPos = actingCamera->getPosition();
    // actingCamera->setPosition(Ogre::Vector3(
    //     // Ogre::Math::Clamp(cameraPos.x, -(Wall::GRID_SIZE * 0.5f) + 3.0f, Wall::GRID_SIZE * 0.5f - 3.0f),
    //     // Ogre::Math::Clamp(cameraPos.y, -(Wall::GRID_SIZE * 0.5f) + 3.0f, Wall::GRID_SIZE * 0.5f - 3.0f),
    //     cameraPos.z
    // ));
    // mSceneMgr->setShadowFarDistance(actingCamera->getPosition().z + Wall::GRID_SIZE);
    
     
    // if (isHost) {
        if(simulator != NULL) {
            simulator->dynamicsWorld->stepSimulation(simulator->physicsClock->getTimeSeconds());
            simulator->physicsClock->reset();

            // btTransform ballTransform;
            // mBall->motionState->getWorldTransform(ballTransform);
            // btVector3 origin = ballTransform.getOrigin();

            // if (origin.z() > Wall::GRID_SIZE) {
            //     reset(ballTransform, origin);
            // }

            // // simulator->dynamicsWorld->contactPairTest(mBall->body, mWall->body, *mBallScoreCallback);
            // if(mBall->colliding) {
            //     // bool away = mBall->rootNode->getPosition().distance(mWall->rootNode->getPosition()) > Wall::GRID_SIZE / 4;
            //     if(collisionClock->getTimeSeconds() > .3 && away) {
            //         mBall->colliding = false;
            //     }
            // }
            // // simulator->dynamicsWorld->contactPairTest(mBall->body, bWall->body, *mBallFloorCallback);
            // if(mBall->f_colliding) {
            //     // bool away = mBall->rootNode->getPosition().distance(bWall->rootNode->getPosition()) > Wall::GRID_SIZE / 4;
            //     if(collisionClock->getTimeSeconds() > .3 && away) {
            //         mBall->f_colliding = false;
            //     }
            // }

            // btVector3 vel = mBall->body->getLinearVelocity();
            // btScalar newX = Ogre::Math::Clamp(vel.getX(), -40.0f, 40.0f);
            // btScalar newY = Ogre::Math::Clamp(vel.getY(), -40.0f, 40.0f);
            // btScalar newZ = Ogre::Math::Clamp(vel.getZ(), -40.0f, 40.0f);
        
            // mBall->body->setLinearVelocity(btVector3(newX, newY, newZ));
            // simulator->dynamicsWorld->contactPairTest(mBall->body, mPaddle->body, *mBallPaddleCallback);
            // simulator->dynamicsWorld->contactPairTest(mBall->body, tWall->body, *mWallCallback);
            // simulator->dynamicsWorld->contactPairTest(mBall->body, rWall->body, *mWallCallback);
            // simulator->dynamicsWorld->contactPairTest(mBall->body, lWall->body, *mWallCallback);
        }

        // btVector3 btCurrentPos = mBall->getPosition();

        // Ogre::Vector3 currentPos(btCurrentPos.x(), btCurrentPos.y(), btCurrentPos.z());// = mBall->rootNode->getPosition();
        // char packet[sizeof(Ogre::Vector3)];
        // memcpy(packet, &currentPos, sizeof(Ogre::Vector3));
        // const char *buf = packet;
        // network->messageClients(PROTOCOL_TCP, buf, sizeof(Ogre::Vector3));
    // }

    // if (network->pollForActivity(1)) {
    //     printf("Activity detected\n");
        // if (isHost) {
    //         ClientData* data = network->tcpClientData[0];
    //         if (data != NULL && strlen(data->output) != 0) {

    //             started = true;
    //             mBall->body->applyCentralImpulse(btVector3(0.0f, 0.0f, -(atoi(data->output))));

    //         }
    //     } 
    //     // else {
    //     //     ClientData* serverData = &network->tcpServerData;
    //     //     if (serverData != NULL && strlen(serverData->output) != 0) {
    //     //         printf("%s\n", serverData->output);
    //     //         const char *buf = serverData->output;
    //     //         Ogre::Vector3 *temp = new Ogre::Vector3();
    //     //         memcpy(temp, buf, sizeof(Ogre::Vector3));
    //     //         mBall->moveTo(*temp);
    //     //         delete temp;
    //     //     }
    //     // }
    // }

    return true;
}

void BallGame::setupCEGUI(void) {
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
    // CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    
    // CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    // CEGUI::Window* myRoot = wmgr.loadLayoutFromFile( "menu.layout" );
    // CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow( myRoot );
    // CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");
    // CEGUI::FontManager::getSingleton().createFreeTypeFont( "DejaVuSans-12", 12, true, "DejaVuSans.ttf", "Fonts");
    // CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont( "DejaVuSans-12" );

    // myRoot->getChild("Host")->subscribeEvent(CEGUI::PushButton::EventClicked, 
    //     CEGUI::Event::Subscriber(&BallGame::hostClick, this));

        
    // printf("%s ughhhh\n", myRoot->getChild("Host")->getText().c_str());
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

void BallGame::setupNetwork()
{
    printf("isHost: %s\n", isHost ? "true" : "false");
    network = new NetManager();
    network->initNetManager();
    if (isHost)
    {
        network->addNetworkInfo(PROTOCOL_TCP, NULL, 51215);
        network->acceptConnections();
        printf("HostName: %s\n", network->getIPstring().c_str());
        network->startServer();
    } 
    else
    {
        network->addNetworkInfo(PROTOCOL_TCP, ipAddr->c_str(), 51215);
        network->startClient();
    }
}

void BallGame::createScene(void)
{
    setupCEGUI();
    setupSDL();
    setupNetwork();
    addResources();
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

    mField = new Field(mSceneMgr, simulator);
    mBall = new Ball(ballString, mSceneMgr, simulator);
    mPaddle = new Paddle(mSceneMgr, simulator);

    new Goal(mSceneMgr, simulator);
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    Ogre::Light* light = mSceneMgr->createLight("SpotLight");
    light->setPosition(Ogre::Vector3(0, 10, 0));
    mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    light->setCastShadows(true);
    scoreObj = new Score();

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


bool BallGame::hostClick(const CEGUI::EventArgs &e) {
    printf("hello!\n");
    return true;
}

bool BallGame::mouseReleased(const OIS::MouseEvent &ev, OIS::MouseButtonID id) {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton);
}

bool BallGame::mousePressed(const OIS::MouseEvent &ev, OIS::MouseButtonID id) {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton);
}

void BallGame::createViewports(void)
{
    Ogre::Camera* camera = isHost ? hostCamera : mCamera;
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(camera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
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
                    printf("ip: %s\n", app.ipAddr->c_str());
                    app.isHost = false;
                    break;

            }
        }
        if (app.ipAddr == NULL) {
            app.isHost = true;
            printf("isHost: true\n");
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
