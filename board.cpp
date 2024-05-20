#include <cstring>
#include <iostream>

#include "bits.h"
#include "board.h"
#include "hash.h"
#include "util.h"

Board::Board() {
    std::memset(pieces, EMPTY, sizeof(pieces));
    std::memset(piece_bbs, 0, sizeof(piece_bbs));
    std::memset(levels, 0, sizeof(levels));
    levels_bb = bb_row(0);
    hash = null_hash;
}

void Board::do_move(Col move_col) {
    
    assert(move_col >= 0);
    assert(move_col < N_COLS);

    const Row move_row = levels[move_col];
    
    assert(move_row >= 0);
    assert(move_row < N_ROWS);
    
    const BB move_bb = bb_of(square_of(move_row, move_col));
    levels_bb ^= (move_bb | move_bb << N_COLS);

    pieces[move_row][move_col] = stm;
    piece_bbs[stm] |= bb_of(square_of(move_row, move_col));
    hash ^= hashes[move_row][move_col][stm];

    levels[move_col]++;

    stm = opposite(stm);
    ply++;


}

void Board::undo_move(Col move_col) {
    
    ply--;
    stm = opposite(stm);

    assert(move_col >= 0);
    assert(move_col < N_COLS);

    levels[move_col]--;
    
    const Row move_row = levels[move_col];

    assert(move_row >= 0);
    assert(move_row < N_ROWS);

    const BB move_bb = bb_of(square_of(move_row, move_col));
    levels_bb ^= (move_bb | move_bb << N_COLS);

    hash ^= hashes[levels[move_col]][move_col][stm];
    piece_bbs[stm] &= ~bb_of(square_of(move_row, move_col));
    pieces[levels[move_col]][move_col] = EMPTY;

}

Result Board::check_result() {

    Piece sjm = opposite(stm);

    BB sjm_bb = piece_bbs[sjm];
    BB stm_bb = piece_bbs[stm];

    constexpr BB shiftout_left  = bb_col(N_COLS - 1) | bb_col(N_COLS - 2) | bb_col(N_COLS - 3);
    constexpr BB shiftout_right = bb_col(0) | bb_col(1) | bb_col(2);

    assert(sjm_bb & ~bb_full == 0);
    assert(stm_bb & ~bb_full == 0);

    // the following code assumes N_CONNECT = 4
    static_assert(N_CONNECT == 4);

    // check downward
    if (bb_four_of_four(sjm_bb, 
                        sjm_bb >> 1 * N_COLS, 
                        sjm_bb >> 2 * N_COLS, 
                        sjm_bb >> 3 * N_COLS))
        return WIN;
    
    // check horizontally
    if (bb_four_of_four(sjm_bb, 
                        sjm_bb >> 1, 
                        sjm_bb >> 2, 
                        sjm_bb >> 3)
        & ~shiftout_left)
        return WIN;

    // check diagonally down+left/up+right
    if (bb_four_of_four(sjm_bb, 
                        sjm_bb >> 1 * (N_COLS + 1), 
                        sjm_bb >> 2 * (N_COLS + 1), 
                        sjm_bb >> 3 * (N_COLS + 1))
        & ~shiftout_left)
        return WIN;

    // check diagonally down+right/up+left
    if (bb_four_of_four(sjm_bb, 
                        sjm_bb >> 1 * (N_COLS - 1), 
                        sjm_bb >> 2 * (N_COLS - 1), 
                        sjm_bb >> 3 * (N_COLS - 1))
        & ~shiftout_right)
        return WIN;

    // check for draw
    if (ply == N_SQUARES)
        return DRAW;

    return NONE;








    // check downward
    if (stm_bb
        & stm_bb >> 1 * N_COLS
        & stm_bb >> 2 * N_COLS
        & levels_bb >> 3 * N_COLS)
        return LOSS + 1;
    
    // check horizontally 1 
    if (levels_bb
        & stm_bb >> 1
        & stm_bb >> 2
        & stm_bb >> 3
        & ~(bb_col(N_COLS - 1) | bb_col(N_COLS - 2) | bb_col(N_COLS - 3)))
        return LOSS + 1;

    // check horizontally 2
    if (stm_bb
        & levels_bb >> 1
        & stm_bb >> 2
        & stm_bb >> 3
        & ~(bb_col(N_COLS - 1) | bb_col(N_COLS - 2) | bb_col(N_COLS - 3)))
        return LOSS + 1;
    
    // check horizontally 3
    if (stm_bb
        & stm_bb >> 1
        & levels_bb >> 2
        & stm_bb >> 3
        & ~(bb_col(N_COLS - 1) | bb_col(N_COLS - 2) | bb_col(N_COLS - 3)))
        return LOSS + 1;
    
    // check horizontally 4
    if (stm_bb
        & stm_bb >> 1
        & stm_bb >> 2
        & levels_bb >> 3
        & ~(bb_col(N_COLS - 1) | bb_col(N_COLS - 2) | bb_col(N_COLS - 3)))
        return LOSS + 1;

    // check diagonally down+left/up+right 1
    if (levels_bb
        & stm_bb >> 1 * (N_COLS + 1)
        & stm_bb >> 2 * (N_COLS + 1)
        & stm_bb >> 3 * (N_COLS + 1)
        & ~(bb_col(N_COLS - 1) | bb_col(N_COLS - 2) | bb_col(N_COLS - 3)))
        return LOSS + 1;

    // check diagonally down+left/up+right 2
    if (stm_bb
        & levels_bb >> 1 * (N_COLS + 1)
        & stm_bb >> 2 * (N_COLS + 1)
        & stm_bb >> 3 * (N_COLS + 1)
        & ~(bb_col(N_COLS - 1) | bb_col(N_COLS - 2) | bb_col(N_COLS - 3)))
        return LOSS + 1;

    // check diagonally down+left/up+right 3
    if (stm_bb
        & stm_bb >> 1 * (N_COLS + 1)
        & levels_bb >> 2 * (N_COLS + 1)
        & stm_bb >> 3 * (N_COLS + 1)
        & ~(bb_col(N_COLS - 1) | bb_col(N_COLS - 2) | bb_col(N_COLS - 3)))
        return LOSS + 1;

    // check diagonally down+left/up+right 4
    if (stm_bb
        & stm_bb >> 1 * (N_COLS + 1)
        & stm_bb >> 2 * (N_COLS + 1)
        & levels_bb >> 3 * (N_COLS + 1)
        & ~(bb_col(N_COLS - 1) | bb_col(N_COLS - 2) | bb_col(N_COLS - 3)))
        return LOSS + 1;

    // check diagonally down+right/up+left 1
    if (levels_bb
        & stm_bb >> 1 * (N_COLS - 1)
        & stm_bb >> 2 * (N_COLS - 1)
        & stm_bb >> 3 * (N_COLS - 1)
        & ~(bb_col(0) | bb_col(1) | bb_col(2)))
        return LOSS + 1;


    // check diagonally down+right/up+left 2
    if (stm_bb
        & levels_bb >> 1 * (N_COLS - 1)
        & stm_bb >> 2 * (N_COLS - 1)
        & stm_bb >> 3 * (N_COLS - 1)
        & ~(bb_col(0) | bb_col(1) | bb_col(2)))
        return LOSS + 1;

    // check diagonally down+right/up+left 3
    if (stm_bb
        & stm_bb >> 1 * (N_COLS - 1)
        & levels_bb >> 2 * (N_COLS - 1)
        & stm_bb >> 3 * (N_COLS - 1)
        & ~(bb_col(0) | bb_col(1) | bb_col(2)))
        return LOSS + 1;

    // check diagonally down+right/up+left 4
    if (stm_bb
        & stm_bb >> 1 * (N_COLS - 1)
        & stm_bb >> 2 * (N_COLS - 1)
        & levels_bb >> 3 * (N_COLS - 1)
        & ~(bb_col(0) | bb_col(1) | bb_col(2)))
        return LOSS + 1;

    return NONE;

}