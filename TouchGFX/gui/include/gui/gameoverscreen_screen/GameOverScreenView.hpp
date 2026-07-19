#ifndef GAMEOVERSCREENVIEW_HPP
#define GAMEOVERSCREENVIEW_HPP

#include <gui_generated/gameoverscreen_screen/GameOverScreenViewBase.hpp>
#include <gui/gameoverscreen_screen/GameOverScreenPresenter.hpp>

class GameOverScreenView : public GameOverScreenViewBase
{
public:
    GameOverScreenView();
    virtual ~GameOverScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen() {}

private:
    touchgfx::Unicode::UnicodeChar scoreBuf[12];
};

#endif
