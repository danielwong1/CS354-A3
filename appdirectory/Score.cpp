#include "Score.h"
#include <string>
#include <sstream>


Score::Score()
{
    score = 0;
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "Score/Sheet");

    scoreWindow = wmgr.createWindow("TaharezLook/StaticText", "Score/StaticText");
    std::stringstream scoreString;
	scoreString << " Score: " << score;
    scoreWindow->setText(scoreString.str());

    scoreWindow->setSize(CEGUI::USize(CEGUI::UDim(0.085, 0), CEGUI::UDim(0.05, 0)));
    scoreWindow->setPosition(CEGUI::UVector2(CEGUI::UDim( 0.01f, 0 ), CEGUI::UDim( 0.01f, 0 ) ) );
    sheet->addChild(scoreWindow);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

void Score::setScore(int count) {
    score = count;
    std::stringstream scoreString;
	scoreString << " Score: " << score;
    scoreWindow->setText(scoreString.str());
}