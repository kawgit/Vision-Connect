#include <cstring>
#include <iostream>

#include "board.h"
#include "hash.h"

Board::Board() {
    std::memset(squares, EMPTY, sizeof(squares));
    std::memset(levels, 0, sizeof(levels));
    hash = null_hash;
}

void Board::do_move(Col move_col) {

    assert(move_col >= 0);
    assert(move_col < N_COLS);

    squares[levels[move_col]][move_col] = stm;
    hash ^= hashes[levels[move_col]][move_col][stm];

    last_col = move_col;
    last_row = levels[move_col];
    
    levels[move_col]++;

    stm = opposite(stm);
    ply++;


}

void Board::undo_move(Col move_col) {
    
    assert(move_col >= 0);
    assert(move_col < N_COLS);

    ply--;
    stm = opposite(stm);

    levels[move_col]--;

    last_col = -1;
    last_row = -1;

    squares[levels[move_col]][move_col] = EMPTY;
    hash ^= hashes[levels[move_col]][move_col][stm];

}

Result Board::check_result() {

    if (!ply) return NONE;
    
    assert(last_row != -1);
    assert(last_col != -1);

    Piece move_piece = opposite(stm);

    assert(squares[last_row][last_col] == move_piece);

    // check downward
    {
        int cnt = 1;

        for (int row = last_row - 1; row >= 0 && squares[row][last_col] == move_piece; row--)
            cnt++;
        
        if (cnt >= N_CONNECT)
            return WIN;
    }

    // check horizontally
    {
        int cnt = 1;
        
        for (int col = last_col + 1; col < N_COLS && squares[last_row][col] == move_piece && cnt < N_CONNECT; col++)
            cnt++;

        for (int col = last_col - 1; col >= 0 && squares[last_row][col] == move_piece && cnt < N_CONNECT; col--)
            cnt++;

        if (cnt >= N_CONNECT)
            return WIN;
    }

    // check y=x diagonally
    {
        int cnt = 1;

        for (int i = 1; last_row + i < N_ROWS && last_col + i < N_COLS && squares[last_row + i][last_col + i] == move_piece && cnt < N_CONNECT; i++)
            cnt++;
        
        for (int i = 1; last_row - i >= 0     && last_col - i >= 0     && squares[last_row - i][last_col - i] == move_piece && cnt < N_CONNECT; i++)
            cnt++;

        if (cnt >= N_CONNECT)
            return WIN;
    }

    // check y=-x diagonally
    {
        int cnt = 1;

        for (int i = 1; last_row + i < N_ROWS && last_col - i >= 0     && squares[last_row + i][last_col - i] == move_piece && cnt < N_CONNECT; i++)
            cnt++;
        
        for (int i = 1; last_row - i >= 0     && last_col + i < N_COLS && squares[last_row - i][last_col + i] == move_piece && cnt < N_CONNECT; i++)
            cnt++;

        if (cnt >= N_CONNECT)
            return WIN;
    }

    // check for draw
    if (ply == N_ROWS * N_COLS)
        return DRAW;

    return NONE;

}