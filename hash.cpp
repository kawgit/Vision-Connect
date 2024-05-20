#include <cstdlib>
#include <cstring>
#include <iostream>

#include "hash.h"

Hash null_hash;
Hash hashes[N_ROWS][N_COLS][N_SIDES];
Entry tt[N_ENTRIES];

uint64_t rand64() {
    uint64_t result = 0;
    for (int i = 0; i < 64 / 15 + 1; i++) {
        result <<= 15;
        result ^= rand();
    }
    return result;
}

void init_hashes() {

    null_hash = rand64();
    
    for (Row row = 0; row < N_ROWS; row++)
        for (Col col = 0; col < N_COLS; col++)
            for (Piece side = 0; side < N_SIDES; side++)
                hashes[row][col][side] = rand64();

}

void Entry::update(Hash new_hash, Result new_result, Col new_move, Bound new_bound, Depth new_depth) {
    uint32_t new_hash32 = new_hash >> 32;
    if (new_hash32 == hash32
        || new_depth >= depth
        || new_bound > bound) {

        hash32 = new_hash32;
        result = new_result;
        move   = new_move;
        bound  = new_bound;
        depth  = new_depth;
    }
}

Entry* lookup(Hash hash, bool& found) {
    Entry* entry = &tt[hash & (N_ENTRIES - 1)];
    found = uint32_t(entry->hash32) == uint32_t(hash>>32);
    return entry;
}

void reset_tt() {
    std::memset(tt, 0, sizeof(tt));
}