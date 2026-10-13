#include "sprite_grid.cpp"
#include <cstdio>

int main() {
    int bad = 0;
    // 3600 bytes on the EE means 900 four-byte pointer cells, 30 per row.
    bad |= sizeof(sprGrid) / sizeof(sprGrid[0][0]) != 900 || sizeof(sprGrid[0]) / sizeof(sprGrid[0][0]) != 30;
    bad |= sizeof(spriteCount) != 4 || spriteCount != 0;
    for (int r = 0; r < 30; ++r)
        for (int c = 0; c < 30; ++c) bad |= sprGrid[r][c] != 0;
    std::printf(bad ? "FAIL\n" : "ok: 30x30 cells and zero start\n");
    return bad;
}
