#include "BallBoundaryCallback.h"
#include "BallGame.h"
#include "Ball.h"
#include "Score.h"
#include "src/audio.h"
/**
 * Constructs the BallBoundaryCallback
 *
 * @constructor
 * @param pBall
 */
BallBoundaryCallback::BallBoundaryCallback(BallGame* ballGame, NetManager* network) : 
bGame(ballGame), network(network) {}

/**
 * Callback for whenever the ball collides with the Score
 *
 */
btScalar BallBoundaryCallback::addSingleResult(btManifoldPoint& cp,
	const btCollisionObjectWrapper* colObj0Wrap,
	int partId0,
	int index0,
	const btCollisionObjectWrapper* colObj1Wrap,
	int partId1,
	int index1) {
	
	char* buffer = new char[sizeof(MessageType) + sizeof(ScoreMessage)];
	MessageType mType = SCORE;
	memcpy(buffer, &mType, sizeof(MessageType));
	ScoreMessage message;
	message.player = 0; // for host

	memcpy(buffer + sizeof(MessageType), &message, sizeof(ScoreMessage));
	network->messageClients(PROTOCOL_TCP, buffer, sizeof(ScoreMessage) + sizeof(MessageType));


	if(bGame->soundOn) { 
		playSound("sounds/ball_hit_wall.wav", SDL_MIX_MAXVOLUME/15);
	}

    // increase opponent score
    bGame->hostScore += 1;
    std::stringstream scoreString;
    scoreString << "Your Score: " << bGame->hostScore;
    bGame->scoreRoot->getChild("Score1")->setText(scoreString.str());\
    bGame->reset();

    delete buffer;
}