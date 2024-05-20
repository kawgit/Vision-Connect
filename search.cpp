#include <array>
#include <iostream>

#include "hash.h"
#include "search.h"
#include "util.h"

uint64_t nodes;

constexpr std::array<int, N_COLS> default_column_order = []() {
    std::array<int, N_COLS> result {};
    int top = 0;
    int bot = -1;
    bool is_top = false;
    for (int i = 0; i < N_COLS; i++) {
        result[i] = (N_COLS + 1) / 2 + (is_top ? top++ : bot--);
        is_top = !is_top;
    }
    return result;
}();

Result search(Board& board, Depth depth, Result alpha, Result beta) {

    nodes++;

    if (beta < DRAW) {
        beta--;
        if (alpha >= beta)
            return beta;
    }
    
    bool found;
    Entry* const entry = lookup(board.hash, found);
    if (found && entry->result != NONE && entry->depth >= depth) {

        if (entry->bound == EXACT && entry->depth >= depth + 2)
            alpha = entry->result - 2;
        else if (entry->bound == UB && entry->result + 2 <= alpha)
            return alpha; 
        else if (entry->bound == LB && entry->result - 2 > alpha) {
            alpha = entry->result - 2;
    
            if (alpha >= beta)
                return beta;
        }
        
    }

    if (depth <= 0)
        return DRAW;

    Result best_result = -INF;
    Col    best_move   = -1;

    for (int i = 0; i < N_COLS; i++) {

        Col move = default_column_order[i];

        if (board.levels[move] == N_ROWS)
            continue;

        board.do_move(move);
        
        Result result = board.check_result();
        if (result == NONE)
            result = -search(board, depth - 1, -beta, -std::max(alpha, best_result));
        
        board.undo_move(move);

        if (result > DRAW + 1)
            result--;

        if (result > best_result) {
            best_result = result;
            best_move   = move;

            if (alpha >= beta)
                break;
        }

    }

    Bound bound = best_result <= alpha ? UB
                : best_result >= beta  ? LB
                : EXACT;

    entry->update(board.hash, best_result, best_move, bound, depth);

    return best_result;

}
