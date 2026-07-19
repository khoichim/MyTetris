#pragma once
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define BOARD_W   10
#define BOARD_H   20
#define CELL_PX   13
#define BOARD_OX  5
#define BOARD_OY  28

enum PieceType { I_P=0, O_P, T_P, S_P, Z_P, J_P, L_P };

static const uint8_t SHAPES[7][4][4] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}, // I
    {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, // O
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // T
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // S
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, // Z
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // J
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // L
};

static const uint32_t COLORS[7] = {
    0x00FFFF, // I Cyan
    0xFFFF00, // O Yellow
    0x9B00FF, // T Purple
    0x00FF00, // S Green
    0xFF0000, // Z Red
    0x0000FF, // J Blue
    0xFF8000, // L Orange
};

struct Piece {
    PieceType type;
    int8_t    x, y;
    uint8_t   shape[4][4];
};

class TetrisGame {
public:
    uint8_t  board[BOARD_H][BOARD_W];
    Piece    current, next;
    uint32_t score;
    uint16_t level, lines;
    bool     gameOver;
    bool     paused;
    uint8_t  tickCount;
    uint8_t  tickLimit;
    uint8_t  ledFlash;
    bool     ledState;
    uint8_t  ledCounter;

    void    init();
    void    tick();
    void    moveLeft();
    void    moveRight();
    void    rotate();
    void    hardDrop();
    void    togglePause();
    int8_t  ghostY() const;
    bool    valid(const Piece& p, int8_t dx, int8_t dy) const;

private:
    void spawn(Piece& p);
    void lock();
    void clearLines();
    void rotatePiece(Piece& p);
};
