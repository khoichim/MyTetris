#ifndef MENUSCREENPRESENTER_HPP
#define MENUSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MenuScreenView;

class MenuScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    MenuScreenPresenter(MenuScreenView& v);

    virtual void activate();
    virtual void deactivate();
    virtual ~MenuScreenPresenter() {}

    void setStartLevel(int l);

private:
    MenuScreenPresenter();
    MenuScreenView& view;
};

#endif
