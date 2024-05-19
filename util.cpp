#include <iostream>

#include "util.h"

const std::string piece_icons[3] = {"X", "O", " "};

void print_board(Board& board) {

    for (int col = 0; col < N_COLS; col++)
        std::cout << "+---";
    std::cout << "+" << std::endl;

    for (int row = N_ROWS - 1; row >= 0; row--) {

        std::cout << "| ";
        for (int col = 0; col < N_COLS; col++)
            std::cout << piece_icons[board.squares[row][col]] << " | ";
        std::cout << std::endl;
        for (int col = 0; col < N_COLS; col++)
            std::cout << "+---";
        std::cout << "+" << std::endl;

    }

    std::cout << "Side to move: " << piece_icons[board.stm] << std::endl;

}

std::string result_to_string(Result result) {

    return (result == NONE ? "NONE"
          : result == DRAW ? "DRAW"
          : result >  DRAW ? ( "WIN in " + std::to_string(WIN - result) + " moves.")
          :                  ("LOSS in " + std::to_string(WIN + result) + " moves."));

}
