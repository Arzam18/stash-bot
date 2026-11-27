/*
**    Stash, a UCI chess playing engine developed from scratch
**    Copyright (C) 2019-2025 Morgan Houppin
**
**    Stash is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    Stash is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STASH_ARCH_ARM_NEON_H
#define STASH_ARCH_ARM_NEON_H

#include <arm_neon.h>

// Counts the set bits in a 64-bit bitboard using AArch64 Advanced SIMD.
// CNT counts set bits independently in each byte; ADDV horizontally sums the
// eight byte counts. This is the NEON equivalent of __builtin_popcountll.
INLINED u32 arm_neon_u64_count_ones(u64 value) {
    return (u32)vaddv_u8(vcnt_u8(vcreate_u8(value)));
}

#endif
