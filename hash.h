#pragma once

#include "types.h"

void init_hashes();

extern Hash null_hash;
extern Hash hashes[N_ROWS][N_COLS][N_SIDES];

struct Entry {
    uint32_t hash32 = 0;
    Result result = NONE;
    Depth depth = -1;
    Col move;
    Bound bound;

    void update(Hash new_hash, Result new_result, Col new_move, Bound new_bound, Depth new_depth);
};

Entry* lookup(Hash hash, bool& found);

extern Entry tt[N_ENTRIES];

void reset_tt();
