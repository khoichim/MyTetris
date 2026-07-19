#include "TetrisGame.hpp"

void TetrisGame::init()
{
    memset(board, 0, sizeof(board));
    score      = 0;
    level      = 1;
    lines      = 0;
    gameOver   = false;
    paused     = false;
    tickCount  = 0;
    tickLimit  = 48;
    ledFlash   = 0;
    ledState   = false;
    ledCounter = 0;

    spawn(next);
    spawn(current);
}

void TetrisGame::spawn(Piece& p)
{
    p.type = (PieceType)(rand() % 7);
    memcpy(p.shape, SHAPES[p.type], sizeof(p.shape));
    p.x = 3;
    p.y = 0;
}

bool TetrisGame::valid(const Piece& p, int8_t dx, int8_t dy) const
{
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++) {
            if (!p.shape[r][c]) continue;
            int nx = p.x + c + dx;
            int ny = p.y + r + dy;
            if (nx < 0 || nx >= BOARD_W || ny >= BOARD_H) return false;
            if (ny >= 0 && board[ny][nx]) return false;
        }
    return true;
}

void TetrisGame::rotatePiece(Piece& p)
{
    uint8_t tmp[4][4] = {};
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            tmp[c][3-r] = p.shape[r][c];
    memcpy(p.shape, tmp, sizeof(tmp));
}

void TetrisGame::lock()
{
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++) {
            if (!current.shape[r][c]) continue;
            int ny = current.y + r;
            int nx = current.x + c;
            if (ny < 0) { gameOver = true; return; }
            board[ny][nx] = current.type + 1;
        }

    clearLines();
    current = next;
    spawn(next);
    if (!valid(current, 0, 0))
        gameOver = true;
}

void TetrisGame::clearLines()
{
    int cleared = 0;
    for (int r = BOARD_H - 1; r >= 0; r--) {
        bool full = true;
        for (int c = 0; c < BOARD_W; c++)
            if (!board[r][c]) { full = false; break; }
        if (full) {
            for (int rr = r; rr > 0; rr--)
                memcpy(board[rr], board[rr-1], BOARD_W);
            memset(board[0], 0, BOARD_W);
            cleared++;
            r++;
        }
    }

    if (cleared > 0) {
        static const uint16_t PT[] = {0, 100, 300, 500, 800};
        score    += PT[cleared] * level;
        lines    += cleared;
        level     = lines / 10 + 1;
        tickLimit = (level < 10) ? (48 - level * 4) : 8;

        // Kích hoạt nháy LED
        ledFlash   = cleared * 2;
        ledCounter = 0;
    }
}

void TetrisGame::tick()
{
    if (gameOver || paused) return;

    // Xử lý LED flash
    if (ledFlash > 0) {
        if (++ledCounter >= 8) {
            ledCounter = 0;
            ledState   = !ledState;
            ledFlash--;
        }
    } else {
        ledState = false;
    }

    // Rơi tự động
    if (++tickCount >= tickLimit) {
        tickCount = 0;
        if (valid(current, 0, 1))
            current.y++;
        else
            lock();
    }
}

void TetrisGame::moveLeft()
{
    if (gameOver || paused) return;
    if (valid(current, -1, 0)) current.x--;
}

void TetrisGame::moveRight()
{
    if (gameOver || paused) return;
    if (valid(current, 1, 0)) current.x++;
}

void TetrisGame::rotate()
{
    if (gameOver || paused) return;
    Piece t = current;
    rotatePiece(t);
    if      (valid(t,  0, 0)) { current = t; }
    else if (valid(t,  1, 0)) { t.x += 1; current = t; }
    else if (valid(t, -1, 0)) { t.x -= 1; current = t; }
    else if (valid(t,  2, 0)) { t.x += 2; current = t; }
}

void TetrisGame::hardDrop()
{
    if (gameOver || paused) return;
    while (valid(current, 0, 1)) { current.y++; score += 2; }
    lock();
}

void TetrisGame::togglePause()
{
    if (!gameOver) paused = !paused;
}

int8_t TetrisGame::ghostY() const
{
    Piece g = current;
    while (valid(g, 0, 1)) g.y++;
    return g.y;
}
