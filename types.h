#pragma once

#include <cassert>

#include "settings.h"


// Non-adjustable constants
constexpr int N_SIDES = 2;
constexpr int N_ENTRIES = 1 << HASH_SIZE;
constexpr int N_SQUARES = N_ROWS * N_COLS;

/// Types
typedef uint8_t Piece;
typedef int Result;
typedef int8_t Col;
typedef int8_t Row;
typedef uint64_t Hash;
typedef uint8_t Bound;
typedef int8_t Depth;
typedef uint64_t BB;
typedef int8_t Square;

enum Pieces : Piece { WHITE, BLACK, EMPTY };
enum Results : Result { DRAW=0, WIN=1000, LOSS=-WIN, INF=WIN+100, NONE=INF+1 };
enum Bounds : Bound { UB, LB, EXACT };

constexpr Piece opposite(const Piece piece) {
    assert(piece == WHITE || piece == BLACK);
    return piece ^ 1;
}

constexpr Square square_of(const Row row, const Col col) {
    return row * N_COLS + col;
}