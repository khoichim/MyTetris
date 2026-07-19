#ifndef GAMESCREENPRESENTER_HPP
#define GAMESCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class GameScreenView;

class GameScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    GameScreenPresenter(GameScreenView& v);

    virtual void activate();
    virtual void deactivate();
    virtual ~GameScreenPresenter() {}

    void     setFinalScore(uint32_t s);
    uint32_t getFinalScore();
    int      getStartLevel();

private:
    GameScreenPresenter();
    GameScreenView& view;
    uint32_t        finalScore;
};

#endif
