#include "../w10-world-b-common/mem.hpp"
// Observed gameLineFuse__Fv at 0x00278480, 1012 bytes in SLUS_205.65. Takes no argument and
// returns nothing. Joins pairs of game lines (gameLines, 0x008d57a8; gameLineCount at
// 0x00485ea8) whose ends meet, replacing each pair by one new line object.
// A line record has its name pointer at +0, its vertex count at +4, a word at +8, and its
// vertex array at +20 (64-byte vertices whose first three floats are the position).
// For each line A in table order (index i), over every other line B (index j != i) that has
//   the same word at +8 and an equal name (strcmp of the two +0 pointers is zero),
// the distance from A's last vertex to B's first vertex is measured; B is a candidate when
// that distance is below 96.0f and strictly below the best so far (the start is 99999992.0f).
// The nearest candidate (the first among equals) is chosen. With no candidate A is skipped.
// With one, the distance from A's first vertex to B's last vertex decides the last argument
// of the new line: 1 when it is below 96.0f, else 0. A 24-byte block is allocated with
// blockAlloc__Fi; when it is not null, __4LineP4LineT1i(block, A, B, flag) builds the new
// line there. Then A is removed from the table (its destructor _$_4Line(A, 3) runs when it is
// found and non-null, the later entries move down one place, the count falls by one), B is
// removed the same way, and the new line pointer (null when the allocation failed) is
// stored at the end of the table, which leaves the count one less than it started. The scan
// then starts again from the first line (a null entry from a failed allocation is read
// through by that scan, as in the image).
// When a scan completes without a fuse, the routine spins for gameLineCount iterations of six
// no-operation instructions (nothing else happens) and returns.
// Inferred: the vertex arrays are path vertices (see gameLineGetVert), the +8 word a closed or
// kind marker, and the flag passed to the constructor whether the joined ends are close
// enough to share a vertex. Unknown: the Line class layout and what the constructor does with
// the flag. The 24-byte size is only the allocation size seen here.
extern "C" int strcmp(const char* left, const char* right);
extern "C" unsigned char* blockAlloc__Fi(int size);
extern "C" void __4LineP4LineT1i(unsigned char* self, unsigned char* first, unsigned char* second, int flag);
extern "C" void _$_4Line(unsigned char* self, int flags);
extern unsigned char* gameLines[100];
extern int gameLineCount;

namespace {
inline __attribute__((always_inline)) float distance_between(const unsigned char* a, const unsigned char* b) {
    using namespace norrath_wb;
    const float dx = load_float(a) - load_float(b);
    const float dy = load_float(a + 4) - load_float(b + 4);
    const float dz = load_float(a + 8) - load_float(b + 8);
    return __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
}

inline __attribute__((always_inline)) unsigned char* last_vertex(unsigned char* line) {
    using namespace norrath_wb;
    return load_pointer(line + 20) + (int(load_word(line + 4)) << 6) - 64;
}

// Removes `line` from gameLines and destroys it; the count falls by one whether or not
// it was found.
inline __attribute__((always_inline)) void remove_line(unsigned char* line) {
    int count = gameLineCount;
    int at = 0;
    while (at < count) {
        if (gameLines[at] == line) {
            if (line != 0) _$_4Line(line, 3);
            count = gameLineCount;
            for (int k = at + 1; k < count; ++k) gameLines[k - 1] = gameLines[k];
            break;
        }
        ++at;
    }
    gameLineCount = gameLineCount - 1;
}
}

extern "C" void gameLineFuse__Fv() {
    using namespace norrath_wb;
    for (;;) {
        int i = 0;
        bool fused = false;
        while (i < gameLineCount) {
            unsigned char* a = gameLines[i];
            int best = -1;
            float bestDistance = 99999992.0f;
            for (int j = 0; j < gameLineCount; ++j) {
                if (j == i) continue;
                unsigned char* b = gameLines[j];
                if (load_word(a + 8) != load_word(b + 8)) continue;
                if (strcmp(reinterpret_cast<const char*>(load_pointer(a)),
                           reinterpret_cast<const char*>(load_pointer(b))) != 0) continue;
                const float d = distance_between(last_vertex(a), load_pointer(b + 20));
                if (!(d < 96.0f)) continue;
                if (d < bestDistance) {
                    bestDistance = d;
                    best = j;
                }
            }
            if (best == -1) {
                ++i;
                continue;
            }
            unsigned char* b = gameLines[best];
            const float closing = distance_between(load_pointer(a + 20), last_vertex(b));
            unsigned char* fresh = blockAlloc__Fi(24);
            if (fresh != 0) __4LineP4LineT1i(fresh, a, b, closing < 96.0f ? 1 : 0);
            remove_line(a);
            remove_line(b);
            gameLines[gameLineCount] = fresh;
            gameLineCount = gameLineCount + 1;
            fused = true;
            break;
        }
        if (!fused) break;
    }
    int k = gameLineCount;
    if (k > 0) {
        do {
            --k;
            asm volatile("nop");
            asm volatile("nop");
            asm volatile("nop");
            asm volatile("nop");
            asm volatile("nop");
            asm volatile("nop");
        } while (k != 0);
    }
}
