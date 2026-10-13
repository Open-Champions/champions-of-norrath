// Observed shopUnpackToGrid__Fiii at 0x001b6e80, 852 bytes (int owner, int screen,
// int shop). Fills, for each of three item lists, a packing grid of 9 columns by 75
// rows of 72-byte item records. Does nothing for a negative owner. With
// R = player + owner * 23984 and, for list k = 0, 1, 2:
//   * source list: shop == 0 -> R + 328 + k * 4616 (the player's own list);
//     otherwise shopList + k * 4616. A list is up to 64 72-byte records followed by
//     a count word at +4608 (read before the grid is touched);
//   * grid: shop == 0 -> invGrid + screen * 145800 + k * 48600; otherwise shopGrid
//     with the same offset. Cell (column c, row r) is at grid + c * 5400 + r * 72.
//   1. Every cell has its first word and the halfword at +34 cleared.
//   2. For each source record i in order: the record's halfword +34 is set to i;
//      getPicSize__4itemiPQ24item11descriptionPiT3(word at R+84, record, &w, &h)
//      gives a pixel size; cols = w / 32 + 1 and rows = (h - 6) / 34 + 1 (signed
//      division). The grid is scanned row-major (rows 0..74 outer, columns 0..8
//      inner) for the first position where column + cols < 10, row + rows < 76 and
//      the first word of every covered cell is zero. If one is found, the whole
//      record is copied to every covered cell and each copy gets halfword +34 = i.
//      If none is found the record is skipped.
// Unknown: what R+84 selects for the picture size, and what the record's first word
// and halfword at +34 mean beyond being the occupancy marker and source index.
extern "C" {
extern unsigned char player[];
extern unsigned char shopList[];
extern unsigned char* invGrid;
extern unsigned char* shopGrid;
void getPicSize__4itemiPQ24item11descriptionPiT3(int, unsigned char*, int*, int*);

struct ItemCell { unsigned char b[72]; };

void shopUnpackToGrid__Fiii(int owner, int screen, int shop) {
    if (owner < 0) return;
    unsigned char* record = player + owner * 23984;
    for (int k = 0; k < 3; k++) {
        unsigned char* source = shop ? shopList + k * 4616 : record + 328 + k * 4616;
        unsigned char* grid = (shop ? shopGrid : invGrid) + screen * 145800 + k * 48600;
        int count = *reinterpret_cast<int*>(source + 4608);
        for (int c = 0; c < 9; c++) {
            for (int r = 0; r < 75; r++) {
                unsigned char* cell = grid + c * 5400 + r * 72;
                *reinterpret_cast<int*>(cell) = 0;
                *reinterpret_cast<unsigned short*>(cell + 34) = 0;
            }
        }
        for (int i = 0; i < count; i++) {
            unsigned char* entry = source + i * 72;
            int w[2];
            getPicSize__4itemiPQ24item11descriptionPiT3(*reinterpret_cast<int*>(record + 84), entry, &w[0], &w[1]);
            *reinterpret_cast<unsigned short*>(entry + 34) = static_cast<unsigned short>(i);
            int cols = w[0] / 32 + 1;
            int rows = (w[1] - 6) / 34 + 1;
            bool placed = false;
            int row = 0, col = 0;
            for (row = 0; row < 75 && !placed; row++) {
                for (col = 0; col < 9; col++) {
                    bool free = true;
                    for (int r = 0; r < rows; r++) {
                        for (int c = 0; c < cols; c++) {
                            if (*reinterpret_cast<int*>(grid + (col + c) * 5400 + (row + r) * 72) != 0) free = false;
                        }
                    }
                    if (col + cols >= 10 || row + rows >= 76) free = false;
                    if (free) { placed = true; break; }
                }
                if (placed) break;
            }
            if (!placed) continue;
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    unsigned char* cell = grid + (col + c) * 5400 + (row + r) * 72;
                    *reinterpret_cast<ItemCell*>(cell) = *reinterpret_cast<ItemCell*>(entry);
                    *reinterpret_cast<unsigned short*>(cell + 34) = static_cast<unsigned short>(i);
                }
            }
        }
    }
}
}
