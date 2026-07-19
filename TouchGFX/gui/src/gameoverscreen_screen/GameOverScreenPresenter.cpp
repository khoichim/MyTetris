#include <gui/gameoverscreen_screen/GameOverScreenView.hpp>
#include <gui/gameoverscreen_screen/GameOverScreenPresenter.hpp>

GameOverScreenPresenter::GameOverScreenPresenter(GameOverScreenView& v)
    : view(v)
{
}

void GameOverScreenPresenter::activate()
{
}

void GameOverScreenPresenter::deactivate()
{
}

uint32_t GameOverScreenPresenter::getFinalScore()
{
    return model->getFinalScore();
}
