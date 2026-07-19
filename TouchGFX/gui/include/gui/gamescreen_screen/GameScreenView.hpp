#ifndef GAMESCREENVIEW_HPP
#define GAMESCREENVIEW_HPP

#include <gui_generated/gamescreen_screen/GameScreenViewBase.hpp>
#include <gui/gamescreen_screen/GameScreenPresenter.hpp>
#include "TetrisGame.hpp"

class GameScreenView : public GameScreenViewBase
{
public:
    GameScreenView() {}
    virtual ~GameScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleGestureEvent(const GestureEvent& evt);

private:
    int8_t  swipeDir;
    uint8_t holdTick;
    bool    btnLastState;

    static const uint8_t HOLD_DELAY  = 8;
    static const uint8_t HOLD_REPEAT = 3;

    touchgfx::Unicode::UnicodeChar scoreBuf[12];
    touchgfx::Unicode::UnicodeChar levelBuf[4];
    touchgfx::Unicode::UnicodeChar linesBuf[4];

    void updateHUD();
    void updateLED();
    void drawBoardCells();
    void drawPiece(const Piece& p, bool ghost);
    void drawNextPiece();
    void fillCell(int px, int py, uint8_t r, uint8_t g, uint8_t b);
};
static_assert(sizeof(GameScreenView) < 2048, "GameScreenView too large!");
#endif
