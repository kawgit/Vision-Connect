#include <iostream>
#include <string>

#include "hash.h"
#include "search.h"
#include "util.h"

int main() {

    init_hashes();

    std::cout << "Hi! Try and beat the bot at connect-4! Wait for the bot to move first, then the number associated with the column you wish to drop your stone in (numbered 0-6 from left to right)." << std::endl;

    Board board;

    while (!board.ply || board.check_result() == NONE) {

        reset_tt();

        for (Depth depth = 1; depth < 17; depth++) {
            std::cout << "*** Depth " << int(depth) << std::endl;
            search(board, depth, -INF, INF);
        }

        bool found;
        Entry* entry = lookup(board.hash, found);
        assert(found);

        Col move = entry->move;
        Result result = entry->result;

        std::cout << "The computer chooses to place a stone in column " << int(move) << std::endl;
        std::cout << "The computer expects to " << result_to_string(result) << std::endl;

        board.do_move(move);

        print_board(board);

        if (board.check_result() != NONE)
            break;

        std::string movestr;
        std::cin >> movestr;
        move = std::stoi(movestr);

        assert(move >= 0 && move < N_COLS && board.levels[move] != N_ROWS);

        board.do_move(move);

        print_board(board);

    }

    if (board.check_result() == WIN) {
        if (board.stm != WHITE)
            std::cout << "Haha you lose. Embarrasssiiiiiiiing." << std::endl;
        else
            std::cout << "You must've cheated." << std::endl;
    }
    else
        std::cout << "Boring." << std::endl;

}