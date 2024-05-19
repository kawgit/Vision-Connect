#include <iostream>
#include <string>

#include "hash.h"
#include "search.h"
#include "util.h"
#include "timer.h"
#include "bits.h"

int main() {

    init_hashes();

    std::cout << "Hi! Try and beat the bot at connect-4! Wait for the bot to move first, then the number associated with the column you wish to drop your stone in (numbered 1-" + std::to_string(N_COLS) + " from left to right)." << std::endl;

    Board board;
    Timestamp start;

    while (!board.ply || (board.check_result() != WIN && board.check_result() != LOSS && board.check_result() != DRAW)) {

        reset_tt();
        nodes = 0;
        start = get_current_ms();

        for (Depth depth = 1; depth < 25; depth++) {
            std::cout << "*** Depth " << int(depth);
            search(board, depth, -INF, INF);
            std::cout << " marginal nodes: " << nodes << " marginal nps: " << nodes / (get_time_diff(start) + 1) << "k" << std::endl; 
        }

        bool found;
        Entry* entry = lookup(board.hash, found);
        assert(found);

        Col move = entry->move;
        Result result = entry->result;

        std::cout << "\n\n\n\n" << std::endl;
        std::cout << "The computer chooses to place a stone in column " << int(move) << std::endl;
        std::cout << "The computer expects to " << result_to_string(result) << std::endl;

        board.do_move(move);

        print_board(board);

        if (board.check_result() == WIN || board.check_result() == LOSS || board.check_result() == DRAW)
            break;

        do {
            std::cout << "Please enter a valid column number (1-" + std::to_string(N_COLS) + ")" << std::endl;
            std::string movestr;
            std::cin >> movestr;
            move = std::stoi(movestr) - 1;
        } while (!(move >= 0 && move < N_COLS && board.levels[move] != N_ROWS));

        board.do_move(move);

        print_board(board);

    }

    std::cout << std::endl;

    if (board.check_result() == WIN) {
        if (board.stm != WHITE)
            std::cout << "Haha you lose. Embarrasssiiiiiiiing." << std::endl;
        else
            std::cout << "You win. You definitely cheated." << std::endl;
    }
    else
        std::cout << "Boring." << std::endl;

}