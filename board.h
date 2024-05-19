#pragma once

#include "types.h"

/// Board Representation

struct Board {

    Piece pieces[N_ROWS][N_COLS];
    BB piece_bbs[N_SIDES];
    Row levels[N_COLS];
    BB levels_bb;
    Piece stm = WHITE;
    int ply = 0;
    Hash hash = 0;

    Board();
    void do_move(Col move_col);
    void undo_move(Col move_col);
    Result check_result();

};