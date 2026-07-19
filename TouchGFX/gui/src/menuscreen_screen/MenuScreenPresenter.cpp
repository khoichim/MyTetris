#include <gui/menuscreen_screen/MenuScreenView.hpp>
#include <gui/menuscreen_screen/MenuScreenPresenter.hpp>

MenuScreenPresenter::MenuScreenPresenter(MenuScreenView& v)
    : view(v)
{
}

void MenuScreenPresenter::activate()
{
}

void MenuScreenPresenter::deactivate()
{
}

void MenuScreenPresenter::setStartLevel(int l)
{
    model->setStartLevel(l);
}
