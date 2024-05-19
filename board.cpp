#include <cstring>
#include <iostream>

#include "bits.h"
#include "board.h"
#include "hash.h"
#include "util.h"

Board::Board() {
    std::memset(mailboxes, EMPTY, sizeof(mailboxes));
    std::memset(levels, 0, sizeof(levels));
    std::memset(piece_bbs, 0, sizeof(piece_bbs));
    hash = null_hash;
}

void Board::do_move(Col move_col) {
    
    assert(move_col >= 0);
    assert(move_col < N_COLS);

    const Row move_row = levels[move_col];
    
    assert(move_row >= 0);
    assert(move_row < N_ROWS);

    mailboxes[move_row][move_col] = stm;
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

    hash ^= hashes[levels[move_col]][move_col][stm];
    piece_bbs[stm] &= ~bb_of(square_of(move_row, move_col));
    mailboxes[levels[move_col]][move_col] = EMPTY;

}

Result Board::check_result() {

    Piece side = opposite(stm);

    BB occ = piece_bbs[side];

    assert(occ & ~bb_full == 0);

    // the following code assumes N_CONNECT = 4
    static_assert(N_CONNECT == 4);

    // check downward
    if (occ
        & occ >> 1 * N_COLS
        & occ >> 2 * N_COLS
        & occ >> 3 * N_COLS)
        return WIN;
    
    // check horizontally
    if (occ
        & occ >> 1
        & occ >> 2
        & occ >> 3
        & ~(bb_col(N_COLS - 1) | bb_col(N_COLS - 2) | bb_col(N_COLS - 3)))
        return WIN;

    // check diagonally down+left/up+right
    if (occ
        & occ >> 1 * (N_COLS + 1)
        & occ >> 2 * (N_COLS + 1)
        & occ >> 3 * (N_COLS + 1)
        & ~(bb_col(N_COLS - 1) | bb_col(N_COLS - 2) | bb_col(N_COLS - 3)))
        return WIN;

    // check diagonally down+right/up+left
    if (occ
        & occ >> 1 * (N_COLS - 1)
        & occ >> 2 * (N_COLS - 1)
        & occ >> 3 * (N_COLS - 1)
        & ~(bb_col(0) | bb_col(1) | bb_col(2)))
        return WIN;

    // check for draw
    if (ply == N_SQUARES)
        return DRAW;

    return NONE;

}