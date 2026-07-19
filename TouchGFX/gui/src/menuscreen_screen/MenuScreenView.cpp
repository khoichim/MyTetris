#include <gui/menuscreen_screen/MenuScreenView.hpp>

void MenuScreenView::setupScreen()
{
    MenuScreenViewBase::setupScreen();
    selectedLevel = 1;
    updateLevelDisplay();
}

void MenuScreenView::tearDownScreen()
{
    MenuScreenViewBase::tearDownScreen();
}

void MenuScreenView::updateLevelDisplay()
{
    touchgfx::Unicode::snprintf(levelBuf, 4, "%d", selectedLevel);
    txtLevel.setWildcard(levelBuf);
    txtLevel.invalidate();
}

void MenuScreenView::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() != ClickEvent::RELEASED) return;

    int x = evt.getX();
    int y = evt.getY();

    // Nút "<" giảm level
    if (x >= 55 && x <= 95 && y >= 168 && y <= 202) {
        if (selectedLevel > 1) {
            selectedLevel--;
            updateLevelDisplay();
        }
    }
    // Nút ">" tăng level
    else if (x >= 145 && x <= 185 && y >= 168 && y <= 202) {
        if (selectedLevel < 5) {
            selectedLevel++;
            updateLevelDisplay();
        }
    }
    // Nút START hoặc bất kỳ chỗ khác
    else {
        presenter->setStartLevel(selectedLevel);
        application().gotoGameScreenScreenNoTransition();
    }
}
