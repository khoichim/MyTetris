#ifndef GAMEOVERSCREENPRESENTER_HPP
#define GAMEOVERSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class GameOverScreenView;

class GameOverScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    GameOverScreenPresenter(GameOverScreenView& v);

    virtual void activate();
    virtual void deactivate();
    virtual ~GameOverScreenPresenter() {}

    // Lấy điểm từ Model
    uint32_t getFinalScore();

private:
    GameOverScreenPresenter();
    GameOverScreenView& view;
};

#endif
