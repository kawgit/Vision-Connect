#pragma once

#include "types.h"

/// Board Representation

struct Board {

    Piece squares[N_ROWS][N_COLS];
    Row levels[N_COLS];
    Piece stm = WHITE;
    int ply = 0;
    Hash hash = 0;

    Col last_col = 0;
    Row last_row = 0;

    Board();
    void do_move(Col move_col);
    void undo_move(Col move_col);
    Result check_result();

};