#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/Color.hpp>
#include "TetrisGame.hpp"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_ltdc.h"

using namespace touchgfx;

void GameScreenView::setupScreen()
{

    bgFull.setTouchable(false);
    boardBg.setTouchable(false);
    sideBg.setTouchable(false);
    nextBg.setTouchable(false);
    txtPause.setTouchable(false);

    swipeDir     = 0;
    holdTick     = 0;
    btnLastState = false;

    int startLv = presenter->getStartLevel();
    if (startLv < 1) startLv = 1;
    if (startLv > 5) startLv = 5;
    memset(g_tetrisGame.board, 0, sizeof(g_tetrisGame.board));
    g_tetrisGame.score      = 0;
    g_tetrisGame.level      = startLv;
    g_tetrisGame.lines      = (startLv - 1) * 10;
    g_tetrisGame.gameOver   = false;
    g_tetrisGame.paused     = false;
    g_tetrisGame.tickCount  = 0;
    g_tetrisGame.tickLimit  = 48 - (startLv - 1) * 4;;
    g_tetrisGame.ledFlash   = 0;
    g_tetrisGame.ledState   = false;
    g_tetrisGame.ledCounter = 0;

    // Seed ngẫu nhiên từ hardware timer
    g_tetrisGame.seedRandom(HAL_GetTick());
    g_tetrisGame.resetBag();
    g_tetrisGame.current.type = I_P;
    g_tetrisGame.current.x    = 3;
    g_tetrisGame.current.y    = 0;
    memcpy(g_tetrisGame.current.shape, SHAPES[I_P], sizeof(SHAPES[I_P]));

    g_tetrisGame.next.type = O_P;
    g_tetrisGame.next.x    = 3;
    g_tetrisGame.next.y    = 0;
    memcpy(g_tetrisGame.next.shape, SHAPES[O_P], sizeof(SHAPES[O_P]));
    txtPause.setVisible(false);
    txtPause.invalidate();
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_RESET);

}

void GameScreenView::tearDownScreen()
{
    GameScreenViewBase::tearDownScreen();
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_RESET);
}

void GameScreenView::handleTickEvent()
{
    // TEST — nháy LED mỗi 60 tick
    static uint32_t cnt = 0;
    if (++cnt == 60) {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
    }
    if (cnt >= 60 && cnt < 70) {
        // giữ sáng 10 tick
    }
    if (cnt >= 70) {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
        cnt = 0;
    }
    // ── PAUSE — PG14
    static bool pauseLast = true;
    static uint8_t pauseCnt = 0;
    bool pauseRaw = (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_14) == GPIO_PIN_RESET);
    if (pauseRaw != pauseLast) {
        pauseCnt++;
        if (pauseCnt >= 3) {
            pauseLast = pauseRaw;
            pauseCnt  = 0;
            if (pauseRaw) {
                g_tetrisGame.togglePause();
                HAL_Delay(32);
            }
        }
    } else { pauseCnt = 0; }

    if (!g_tetrisGame.paused && !g_tetrisGame.gameOver) {

        // ── LEFT — PG2
        static bool leftLast = true;
        bool leftNow = (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_2) == GPIO_PIN_RESET);
        if (leftNow && !leftLast) g_tetrisGame.moveLeft();
        leftLast = leftNow;

        // ── RIGHT — PG3
        static bool rightLast = true;
        bool rightNow = (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_3) == GPIO_PIN_RESET);
        if (rightNow && !rightLast) g_tetrisGame.moveRight();
        rightLast = rightNow;

        // ── DROP — PF6
        static bool dropLast = true;
        static uint8_t dropCnt = 0;
        bool dropRaw = (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6) == GPIO_PIN_RESET);
        if (dropRaw != dropLast) {
            dropCnt++;
            if (dropCnt >= 5) {  // tăng debounce lên 5
                dropLast = dropRaw;
                dropCnt  = 0;
                if (dropRaw) g_tetrisGame.hardDrop();
            }
        } else { dropCnt = 0; }

        // ── ROTATE LEFT — PG9
        static bool rotLLast = true;
        bool rotLNow = (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_9) == GPIO_PIN_RESET);
        if (rotLNow && !rotLLast) g_tetrisGame.rotateLeft();
        rotLLast = rotLNow;

        // ── ROT_R — PB6
        static bool rotRLast = true;
        static uint8_t rotRCnt = 0;
        bool rotRRaw = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_RESET);
        if (rotRRaw != rotRLast) {
            rotRCnt++;
            if (rotRCnt >= 5) {
                rotRLast = rotRRaw;
                rotRCnt  = 0;
                if (rotRRaw) g_tetrisGame.rotate();
            }
        } else { rotRCnt = 0; }
    }

    // Auto-repeat touch (giữ nguyên)
    if (swipeDir != 0 && !g_tetrisGame.paused && !g_tetrisGame.gameOver) {
        holdTick++;
        if (holdTick > HOLD_DELAY &&
           (holdTick - HOLD_DELAY) % HOLD_REPEAT == 0) {
            if (swipeDir == -1) g_tetrisGame.moveLeft();
            else                g_tetrisGame.moveRight();
        }
    }

    g_tetrisGame.tick();

    if (g_tetrisGame.gameOver) {
        presenter->setFinalScore(g_tetrisGame.score);
        application().gotoGameOverScreenScreenNoTransition();
        return;
    }

    updateLED();
    updateHUD();

    // Chỉ vẽ board khi không pause
    if (!g_tetrisGame.paused) {
        drawBoardCells();
        Piece ghost = g_tetrisGame.current;
        ghost.y = g_tetrisGame.ghostY();
        if (ghost.y != g_tetrisGame.current.y)
            drawPiece(ghost, true);
        drawPiece(g_tetrisGame.current, false);
        drawNextPiece();
    }

    txtPause.setVisible(g_tetrisGame.paused);
    txtPause.invalidate();
}

void GameScreenView::handleGestureEvent(const GestureEvent& evt)
{
    if (g_tetrisGame.gameOver || g_tetrisGame.paused) return;

    if (evt.getType() == GestureEvent::SWIPE_HORIZONTAL) {
        // Bỏ kiểm tra velocity — bất kỳ swipe ngang nào cũng di chuyển
        if (evt.getVelocity() < 0) {
            g_tetrisGame.moveLeft();
            swipeDir = -1;
            holdTick = 0;
        } else {
            g_tetrisGame.moveRight();
            swipeDir = 1;
            holdTick = 0;
        }
    }
    else if (evt.getType() == GestureEvent::SWIPE_VERTICAL) {
        if (evt.getVelocity() > 0) {
            g_tetrisGame.hardDrop();  // vuốt xuống = rơi nhanh
        } else {
            g_tetrisGame.rotate();   // vuốt lên = xoay
        }
        swipeDir = 0;
    }
}

void GameScreenView::handleClickEvent(const ClickEvent& evt)
{
    if (g_tetrisGame.gameOver || g_tetrisGame.paused) return;

    if (evt.getType() == ClickEvent::RELEASED) {
        int x = evt.getX();
        int y = evt.getY();

        if (y > 260) {
            // Chạm vùng dưới cùng → hard drop
            g_tetrisGame.hardDrop();
        } else if (x < 60) {
            // Chạm vùng trái → di trái
            g_tetrisGame.moveLeft();
        } else if (x > 180) {
            // Chạm vùng phải → di phải
            g_tetrisGame.moveRight();
        } else {
            // Chạm vùng giữa → xoay
            g_tetrisGame.rotate();
        }
    }
}

void GameScreenView::fillCell(int px, int py, uint8_t r, uint8_t g, uint8_t b)
{
    // Không vẽ khi đang pause


    uint16_t* fb = (uint16_t*)LTDC_Layer1->CFBAR;
    uint16_t color = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
    for (int row = 0; row < CELL_PX - 1; row++)
        for (int col = 0; col < CELL_PX - 1; col++)
            fb[(py + row) * 240 + (px + col)] = color;
}

void GameScreenView::drawBoardCells()
{
    for (int r = 0; r < BOARD_H; r++)
        for (int c = 0; c < BOARD_W; c++) {
            int px = BOARD_OX + c * CELL_PX;
            int py = BOARD_OY + r * CELL_PX;
            uint8_t v = g_tetrisGame.board[r][c];
            if (v > 0) {
                uint32_t rgb = COLORS[v - 1];
                fillCell(px, py, (rgb>>16)&0xFF, (rgb>>8)&0xFF, rgb&0xFF);
            } else {
                fillCell(px, py, 0x10, 0x10, 0x20);
            }
        }
    // Invalidate toàn bộ board 1 lần
    boardBg.invalidate();
}

void GameScreenView::drawPiece(const Piece& p, bool ghost)
{
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++) {
            if (!p.shape[r][c]) continue;
            int br = p.y + r;
            int bc = p.x + c;
            if (br < 0 || br >= BOARD_H || bc < 0 || bc >= BOARD_W) continue;
            int px = BOARD_OX + bc * CELL_PX;
            int py = BOARD_OY + br * CELL_PX;
            uint32_t rgb = COLORS[p.type];
            if (ghost)
                fillCell(px, py,
                    ((rgb>>16)&0xFF)/6,
                    ((rgb>>8)&0xFF)/6,
                    (rgb&0xFF)/6);
            else
                fillCell(px, py,
                    (rgb>>16)&0xFF,
                    (rgb>>8)&0xFF,
                    rgb&0xFF);
        }
}

void GameScreenView::drawNextPiece()
{
    const Piece& n = g_tetrisGame.next;
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++) {
            if (!n.shape[r][c]) continue;
            uint32_t rgb = COLORS[n.type];
            int px = 156 + c * 15;
            int py = 209 + r * 15;
            fillCell(px, py, (rgb>>16)&0xFF, (rgb>>8)&0xFF, rgb&0xFF);
        }
    nextBg.invalidate();
}

void GameScreenView::updateHUD()
{
    // Score
    Unicode::snprintf(scoreBuf, 12, "%d", (int)g_tetrisGame.score);
    scoreVal.setWildcard(scoreBuf);
    scoreVal.invalidate();

    // Level
    Unicode::snprintf(levelBuf, 4, "%d", (int)g_tetrisGame.level);
    levelVal.setWildcard(levelBuf);
    levelVal.invalidate();

    // Lines
    Unicode::snprintf(linesBuf, 4, "%d", (int)g_tetrisGame.lines);
    linesVal.setWildcard(linesBuf);
    linesVal.invalidate();
}

void GameScreenView::updateLED()
{
    if (g_tetrisGame.ledFlash > 0) {
        GPIO_PinState g13 = g_tetrisGame.ledState ? GPIO_PIN_SET   : GPIO_PIN_RESET;
        GPIO_PinState g14 = g_tetrisGame.ledState ? GPIO_PIN_RESET : GPIO_PIN_SET;
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, g13);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, g14);
    } else {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
    }
}
