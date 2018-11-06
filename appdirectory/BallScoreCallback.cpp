#include <SDL2/SDL.h>
#include "BallScoreCallback.h"
#include "BallGame.h"
#include "Ball.h"
#include "Score.h"
#include "src/audio.h"
#include <iostream>

/**
 * Constructs the BallBoundaryCallback
 *
 * @constructor
 * @param pBall
 */
BallScoreCallback::BallScoreCallback(BallGame* pBallGame, NetManager* network) : 
bGame(pBallGame), network(network){}

/**
 * Callback for whenever the ball collides with the Score
 *
 */
btScalar BallScoreCallback::addSingleResult(btManifoldPoint& cp,
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
	message.player = 1; // for host

	memcpy(buffer + sizeof(MessageType), &message, sizeof(ScoreMessage));
	network->messageClients(PROTOCOL_TCP, buffer, sizeof(ScoreMessage) + sizeof(MessageType));


	if(bGame->soundOn) { 
		playSound("sounds/ball_hit_wall.wav", SDL_MIX_MAXVOLUME/15);
	}

    // increase opponent score
    bGame->playerScore += 1;
    std::stringstream scoreString;
    scoreString << "Opponent Score: " << bGame->playerScore;
    bGame->scoreRoot->getChild("Other Score")->setText(scoreString.str());
    bGame->reset();

    delete buffer;

}