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
#include "BallBoundaryCallback.h"
#include "Field.h"
#include "Arrow.h"
#include "Goal.h"
#include "Wall.h"


std::string BallGame::ballString = "ball";
std::string BallGame::botString = "bot";
int BallGame::rotationBound = 400;

BallGame::BallGame() : mRenderer(0)
{
    currentRotationX = 0;
    currentRotationY = 0;
    kicked = false;
}

BallGame::~BallGame(void)
{
    CEGUI::OgreRenderer::destroySystem();
}

void BallGame::reset() {
    kicked = false;
    if (isHost) {
        mBall->moveTo(Ogre::Vector3(0.0, 4.0, 10.0));
        mBall->body->setLinearVelocity(btVector3(0, 0, 0));
        mBall->body->setAngularVelocity(btVector3(0, 0, 0));
    }
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
    
    if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        return false;

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    if(started) {
        double cameraSpeed = 0.005;
        double kickForce = 3.0f;
        Ogre::Camera* camera = isHost ? hostCamera : mCamera;
        Ogre::Radian rotationSpeed = Ogre::Radian(Ogre::Degree(.1));
        Ogre::Vector3 cameraPos = camera->getPosition();

       

        // Arrow controls
        if (!isHost) {
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
        }

        // Kick the ball
        if(mKeyboard->isKeyDown(OIS::KC_SPACE) && !isHost && !kicked) {
            kicked = true;
            Ogre::Vector3 direction = mArrow->rootNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
            direction.normalise();

            MessageType messageType = KICK;

            KickMessage message;
            message.x_val = direction.x;
            message.y_val = direction.y;
            message.z_val = direction.z;

            size_t messageSize = sizeof(KickMessage) + sizeof(MessageType);
            char* buffer = new char[messageSize];
            memcpy(buffer, &messageType, sizeof(MessageType));

            memcpy(buffer + sizeof(MessageType), &message, sizeof(KickMessage));
            network->messageServer(PROTOCOL_TCP, buffer, messageSize);
            delete buffer;
        }


        if(isHost) {
            cameraPos = camera->getPosition();
            camera->setPosition(cameraPos + Ogre::Vector3(cameraSpeed * mRot.x * (isHost ? -1 : 1), -cameraSpeed * mRot.y, 0));
            mPaddle->moveBy(Ogre::Vector3(-cameraSpeed * mRot.x, -cameraSpeed * mRot.y, 0));
            btVector3 position = mPaddle->getPosition();
            mPaddle->moveTo(Ogre::Vector3(
                Ogre::Math::Clamp(position.x(), -10.0f, 10.0f),
                Ogre::Math::Clamp(position.y(), 1.5f, 8.5f),
                position.z()
            ));
            cameraPos = camera->getPosition();
            camera->setPosition(Ogre::Vector3(
                Ogre::Math::Clamp(cameraPos.x, -10.0f, 10.0f),
                Ogre::Math::Clamp(cameraPos.y, 1.5f, 8.5f),
                cameraPos.z
            ));
        }


        mRot = Ogre::Vector2::ZERO;

        // Physics!
        if(simulator != NULL) {
            simulator->dynamicsWorld->stepSimulation(simulator->physicsClock->getTimeSeconds());
            simulator->physicsClock->reset();

            if (isHost) {
                simulator->dynamicsWorld->contactPairTest(mBall->body, lWall->body, *mBallBoundaryCallback);
                simulator->dynamicsWorld->contactPairTest(mBall->body, tWall->body, *mBallBoundaryCallback);
                simulator->dynamicsWorld->contactPairTest(mBall->body, rWall->body, *mBallBoundaryCallback);
                simulator->dynamicsWorld->contactPairTest(mBall->body, cWall->body, *mBallBoundaryCallback);
                simulator->dynamicsWorld->contactPairTest(mBall->body, hWall->body, *mBallBoundaryCallback);
                simulator->dynamicsWorld->contactPairTest(mBall->body, gWall->body, *mBallScoreCallback);
            }
        }

        // Send the position of the ball
        if (isHost) {
            btVector3 ballOrigin = mBall->getPosition();
            btVector3 paddleOrigin = mPaddle->getPosition();

            MessageType messageType = POSITION;

            PositionMessage message;
            message.ballXCoord = ballOrigin.x();
            message.ballYCoord = ballOrigin.y();
            message.ballZCoord = ballOrigin.z();

            message.paddleXCoord = paddleOrigin.x();
            message.paddleYCoord = paddleOrigin.y();
            message.paddleZCoord = paddleOrigin.z();

            size_t messageSize = sizeof(PositionMessage) + sizeof(MessageType);
            char* buffer = new char[messageSize];
            memcpy(buffer, &messageType, sizeof(MessageType));

            memcpy(buffer + sizeof(MessageType), &message, sizeof(PositionMessage));

            network->messageClients(PROTOCOL_TCP, buffer, messageSize);
            delete buffer;
        }
        
        // Networking!
        if (network->pollForActivity(1)) {
            if (isHost) {
                // printf("Host received activity\n");
                ClientData* data = network->tcpClientData[0];
                if (data == NULL) {
                    return true;
                }

                MessageType* messageType = (MessageType*)data->output;
                switch(*messageType) {
                    case KICK: {
                        KickMessage* message = new KickMessage();

                        memcpy(message, data->output + sizeof(MessageType), sizeof(KickMessage));
                        float x = message->x_val;
                        float y = message->y_val;
                        float z = message->z_val;

                        // printf("Host Kick: (%f, %f, %f)\n", x, y, z);
                        mBall->body->applyCentralImpulse(kickForce * btVector3(x, y, z));
                        mBall->body->setActivationState(ACTIVE_TAG);
                        delete message;
                        break;
                    }
                }
            }
            else {
                ClientData* serverData = &network->tcpServerData;
                if (serverData == NULL) {
                    return true;
                }

                MessageType* messageType = (MessageType*)serverData->output;
                switch(*messageType) {
                    case POSITION: {
                        PositionMessage* message = new PositionMessage();

                        memcpy(message, serverData->output + sizeof(MessageType), sizeof(PositionMessage));
                        float x = message->ballXCoord;
                        float y = message->ballYCoord;
                        float z = message->ballZCoord;

                        mBall->moveTo(Ogre::Vector3(x, y, z));

                        x = message->paddleXCoord;
                        y = message->paddleYCoord;
                        z = message->paddleZCoord;
                        mPaddle->moveTo(Ogre::Vector3(x, y, z));

                        delete message;
                        break;
                    }
                    case SCORE: {
                        ScoreMessage* message = new ScoreMessage();

                        memcpy(message, serverData->output + sizeof(MessageType), sizeof(ScoreMessage));
                        int player = message->player;

                        // Host scored
                        if (player == 0) {
                            // increase opponent score
                            hostScore += 1;
                            std::stringstream scoreString;
                            scoreString << "Opponent Score: " << hostScore;
                            scoreRoot->getChild("Other Score")->setText(scoreString.str());
                        }
                        // Client Scored
                        else {
                            // increase my score
                            playerScore += 1;
                            std::stringstream scoreString;
                            scoreString << "Your Score: " << playerScore;
                            scoreRoot->getChild("Score1")->setText(scoreString.str());
                        }

                        reset();
                        break;
                    }
                }
            }
        }
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
    CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    startRoot = wmgr.loadLayoutFromFile( "menu.layout" );
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow( startRoot );
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");
    CEGUI::FontManager::getSingleton().createFreeTypeFont( "DejaVuSans-12", 12, true, "DejaVuSans.ttf", "Fonts");
    CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont( "DejaVuSans-12" );

    startRoot->getChild("Host")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&BallGame::hostClick, this));
    startRoot->getChild("Kicker")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&BallGame::clientClick, this));
}

void BallGame::setupScore(void) {
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    scoreRoot = wmgr.loadLayoutFromFile( "score.layout" );
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow( scoreRoot );
    CEGUI::FontManager::getSingleton().createFreeTypeFont( "DejaVuSans-8", 8, true, "DejaVuSans.ttf", "Fonts");
    CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont( "DejaVuSans-8" );
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
        std::cout << "ip: " << this->ipAddr << std::endl;
        network->addNetworkInfo(PROTOCOL_TCP, ipAddr.c_str(), 51215);
        network->startClient();
    }
}

void BallGame::createScene(void)
{
    started = false;
    createDefaultCamera();
    setupCEGUI();
}

void BallGame::createDefaultCamera(void) {
    mCamera = mSceneMgr->createCamera("MainCam1");
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(.1,.1,.1));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void BallGame::createGame(void) {
    soundOn = true;
    started = true;
    simulator = new Physics();
    collisionClock = new btClock();
    f_collisionClock = new btClock();

    createCamera();
    createViewports();

    setupScore();
    setupSDL();
    setupNetwork();
    addResources();
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

    mField = new Field(mSceneMgr, simulator);
    lWall = new Wall("leftWall", mSceneMgr, simulator, Ogre::Vector3::UNIT_X);
    tWall = new Wall("topWall", mSceneMgr, simulator, Ogre::Vector3::NEGATIVE_UNIT_Y);
    rWall = new Wall("rightWall", mSceneMgr, simulator, Ogre::Vector3::NEGATIVE_UNIT_X);
    cWall = new Wall("clientWall", mSceneMgr, simulator, Ogre::Vector3::NEGATIVE_UNIT_Z);
    hWall = new Wall("hostWall", mSceneMgr, simulator, Ogre::Vector3::UNIT_Z);
    gWall = new GoalieWall("goalieWall", mSceneMgr, simulator);
    mBall = new Ball(ballString, mSceneMgr, simulator);
    //mBall->moveTo(Ogre::Vector3(0.0, 0.0, -Field::SIZE / 2 - 10));
    
    if (!isHost) {
        mArrow = new Arrow(mSceneMgr);
        mArrow->moveArrowTo(Ogre::Vector3(0, 0.2, 10.0));
    }

    
    mBall->moveTo(Ogre::Vector3(0.0, 4.0, 10.0));
    mPaddle = new Paddle(mSceneMgr, simulator);
    mPaddle->moveBy(Ogre::Vector3(0, 5, -29.5));

    mSceneMgr->setSkyBox(true, "sky/Material");
    mGoal = new Goal(mSceneMgr, simulator);
    mGoal->moveBy(Ogre::Vector3(0, 4.5, -25));
    mGoal->rotateBy(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(-90)), Ogre::Vector3::UNIT_Y));

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    Ogre::Light* light = mSceneMgr->createLight("SpotLight");
    light->setPosition(Ogre::Vector3(0, 10, 0));
    mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    light->setCastShadows(true);

    Ogre::Light* light2 = mSceneMgr->createLight("SpotLight1");
    light2->setPosition(Ogre::Vector3(0, 10, -27.5));
    mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    light2->setCastShadows(true);

    createCollisionCallbacks();
}

void BallGame::createCollisionCallbacks(void) {
    mBallScoreCallback = new BallScoreCallback(this, network);
    mBallBoundaryCallback = new BallBoundaryCallback(this, network);
}

bool BallGame::mouseMoved(const OIS::MouseEvent &ev) {
    mRot.x = ev.state.X.rel;
    mRot.y = ev.state.Y.rel;
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(
                ev.state.X.rel, 
                ev.state.Y.rel);
}

void BallGame::destroyArrow(void) {
    if (mArrow) {
        mSceneMgr->getRootSceneNode()->detachObject("root");
        mSceneMgr->destroyEntity("root");
    }
}


bool BallGame::hostClick(const CEGUI::EventArgs &e) {
    this->isHost = true;
    startRoot->hide();

    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide( );
    createGame();
    return true;
}

bool BallGame::clientClick(const CEGUI::EventArgs &e) {
    this->isHost = false;
    //parse ip address
    
    const CEGUI::Editbox* editbox = static_cast<const CEGUI::Editbox*>(startRoot->getChild("Insert"));
    this->ipAddr = editbox->getText().c_str();

    startRoot->hide(); 

    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide( );
    createGame();
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

void BallGame::createViewports(void)
{
    Ogre::Camera* camera = isHost ? hostCamera : mCamera;
    // Create one viewport, entire window
    mWindow->removeAllViewports();
    Ogre::Viewport* vp = mWindow->addViewport(camera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}


void BallGame::go()
{
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
/*
        const struct option long_options[] = {
            {"ip", 1, 0, 'i'},
            {NULL, 0, NULL, 0},
        };
        int option_index = 0;
        int get_opt_err = 0;

        while(get_opt_err != -1) {
            get_opt_err = getopt_long(argc, argv, "i:", long_options, &option_index);
            switch(get_opt_err)
            {
                case 'i': {
                    app.ipAddr = new std::string(optarg);
                    std::cout << "ip: " << app.ipAddr << std::endl;
                    app.isHost = false;
                    break;
                }
            }
        }
        if (app.ipAddr == NULL) {
            app.isHost = true;
        }
*/
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
