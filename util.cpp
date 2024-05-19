#include <iostream>

#include "util.h"
#include "bits.h"

const std::string piece_icons[3] = {"X", "O", " "};

void print_board(Board& board) {

    for (Col col = 0; col < N_COLS; col++)
        std::cout << "+---";
    std::cout << "+" << std::endl;

    for (Row row = N_ROWS - 1; row >= 0; row--) {

        std::cout << "| ";
        for (Col col = 0; col < N_COLS; col++)
            std::cout << piece_icons[board.pieces[row][col]] << " | ";
        std::cout << std::endl;

        for (Col col = 0; col < N_COLS; col++)
            std::cout << "+---";
        std::cout << "+" << std::endl;

    }

    for (Col col = 0; col < N_COLS; col++)
        std::cout << "  " << int(col + 1) << " ";
    std::cout << std::endl;

    std::cout << "Side to move: " << piece_icons[board.stm] << std::endl;

}

void print_bb(BB bb) {

    for (Row row = N_ROWS - 1; row >= 0; row--) {

        for (int col = 0; col < N_COLS; col++)
            std::cout << (bb_has(bb, square_of(row, col)) ? "O" : "-") << " ";

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

std::string result_to_string(Result result) {

    return (result == NONE ? "NONE"
          : result == DRAW ? "DRAW"
          : result >  DRAW ? ( "WIN in " + std::to_string(WIN - result) + " moves.")
          :                  ("LOSS in " + std::to_string(WIN + result) + " moves."));

}
