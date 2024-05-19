#pragma once

#include <array>
#include <cstdlib>

#include "types.h"
	
inline int lsb(const BB n) {
	return __builtin_ctzll(n);
}

inline void dellsb(BB& bb) {
	bb &= bb - 1;
}

inline int poplsb(BB& bb) {
	int i = lsb(bb);
	dellsb(bb);
	return i;
}

inline bool bb_has_multiple(BB bb) {
	dellsb(bb);
	return bool(bb);
}

inline int bitcount(const BB x) {
	return __builtin_popcountll(x);
}

constexpr std::array<BB, N_SQUARES> SQUARE_MASKS = []() constexpr {

    std::array<BB, N_SQUARES> array {};

	BB mask = 1Ull;

    for (size_t i = 0; i < N_SQUARES; i++) {
		array[i] = mask;
		mask <<= 1;
	}

    return array;

}();

inline constexpr BB bb_of(const Square square) {
	return SQUARE_MASKS[square];
}

inline constexpr bool bb_has(const BB bb, const Square square) {
	return bb & bb_of(square);
}

constexpr std::array<BB, N_ROWS> ROW_MASKS = []() constexpr {

    std::array<BB, N_ROWS> array {};

	for (Row row = 0; row < N_ROWS; row++) {
		BB mask = 0;
		for (Col col = 0; col < N_COLS; col++)
			mask |= bb_of(square_of(row, col));
		array[row] = mask;
	}

    return array;

}();


inline constexpr BB bb_row(const Row row) {
	return ROW_MASKS[row];
}

constexpr std::array<BB, N_COLS> COL_MASKS = []() constexpr {

    std::array<BB, N_COLS> array {};

    for (Col col = 0; col < N_COLS; col++) {
		BB mask = 0;
		for (Row row = 0; row < N_ROWS; row++)
			mask |= bb_of(square_of(row, col));
		array[col] = mask;
	}

    return array;

}();

inline constexpr BB bb_col(const Col col) {
	return COL_MASKS[col];
}

constexpr BB bb_full = []() constexpr {
	BB mask = 0;
	for (Row row = 0; row < N_ROWS; row++)
		for (Col col = 0; col < N_COLS; col++)
			mask |= bb_of(square_of(row, col));
	return mask;
}();