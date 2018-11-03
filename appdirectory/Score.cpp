#include "Score.h"
#include <string>
#include <sstream>
#include <iostream>

Score::Score()
{
    clientScore = 0;
    hostScore = 0;
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "Score/Sheet");

    scoreWindow = wmgr.createWindow("TaharezLook/StaticText", "Score/StaticText");
    std::stringstream scoreString;
    setScore();

    scoreWindow->setSize(CEGUI::USize(CEGUI::UDim(0.085, 0), CEGUI::UDim(0.05, 0)));
    scoreWindow->setPosition(CEGUI::UVector2(CEGUI::UDim( 0.01f, 0 ), CEGUI::UDim( 0.01f, 0 ) ) );
    sheet->addChild(scoreWindow);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

void Score::incrementHostScore()
{
    hostScore++;
    setScore();
}

void Score::incrementClientScore()
{
    clientScore++;
    setScore();
}

void Score::setScore()
{
    std::stringstream scoreString;
	scoreString << " Score: " << hostScore << " - " << clientScore;
    scoreWindow->setText(scoreString.str());
}
