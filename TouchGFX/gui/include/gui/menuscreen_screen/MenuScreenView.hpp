#ifndef MENUSCREENVIEW_HPP
#define MENUSCREENVIEW_HPP

#include <gui_generated/menuscreen_screen/MenuScreenViewBase.hpp>
#include <gui/menuscreen_screen/MenuScreenPresenter.hpp>

class MenuScreenView : public MenuScreenViewBase
{
public:
    MenuScreenView() : selectedLevel(1) {}
    virtual ~MenuScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleClickEvent(const ClickEvent& evt);

    int getSelectedLevel() { return selectedLevel; }

private:
    int selectedLevel;
    touchgfx::Unicode::UnicodeChar levelBuf[4];
    void updateLevelDisplay();
};

#endif
