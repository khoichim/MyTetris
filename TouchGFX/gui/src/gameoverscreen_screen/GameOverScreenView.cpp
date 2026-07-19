#include <gui/gameoverscreen_screen/GameOverScreenView.hpp>

GameOverScreenView::GameOverScreenView()
{
}

void GameOverScreenView::setupScreen()
{
    GameOverScreenViewBase::setupScreen();

    uint32_t score = presenter->getFinalScore();

    // Dùng %d thay vì %lu
    touchgfx::Unicode::snprintf(scoreBuf, 12, "%d", (int)score);
    finalScore.setWildcard(scoreBuf);
    finalScore.invalidate();
}
