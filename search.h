#pragma once

#include "board.h"

extern uint64_t nodes;

Result search(Board& board, Depth depth, Result alpha, Result beta);