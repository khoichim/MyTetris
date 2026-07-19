#include <gui/gamescreen_screen/GameScreenPresenter.hpp>
#include <gui/gamescreen_screen/GameScreenView.hpp>

GameScreenPresenter::GameScreenPresenter(GameScreenView& v)
    : view(v), finalScore(0)
{
}

void GameScreenPresenter::activate()
{
}

void GameScreenPresenter::deactivate()
{
}

void GameScreenPresenter::setFinalScore(uint32_t s)
{
    finalScore = s;
    model->setFinalScore(s);
}

uint32_t GameScreenPresenter::getFinalScore()
{
    return model->getFinalScore();
}

int GameScreenPresenter::getStartLevel()
{
    return model->getStartLevel();
}
