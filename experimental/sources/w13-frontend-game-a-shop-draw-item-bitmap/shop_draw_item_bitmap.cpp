// Observed shopDrawItemBitmap__FiP12screenStructfPQ24item11descriptioniii at 0x001b7940, 1892 bytes,
// 224-byte frame. Arguments: side (0 or 1), a screen record, a float y (scroll position), an item cell,
// column, row and mode (nonzero: the shop grid, zero: the player's inventory grid). Screen words:
// [0] left edge, [13] player, [17] [18] floats, the mouse position, [20] tab. The grids are arrays of
// 72-byte cells: cell(col, row) = grid + 5400 * col + 72 * row, with grid = (shopGrid or invGrid) + side *
// 0x23988 + tab * 0xbdd8. The item records of the cell's owner start at p88 = shopList + 4616 * tab
// (mode nonzero) or player + 328 + 23984 * player index + 4616 * tab.
//  1. Cell size: 32 by 34 pixels; with a nonzero word at cell + 0 the picture pic = getPic__4itemi
//     PQ24item11description(halfword record + 84 of the player, cell) is read (width = halfword 0, height =
//     halfword 2) and the size becomes ((width / 32) + 1) * 32 by (((height - 6) / 34) + 1) * 34.
//  2. Two one-pixel-high strips at (screen[0] + 14 + 32 * col) are clipped with shopClipGridCoordinates
//     (x0, y0, x1, y1 as four int pointers): first the strip (+1 .. +33, Y .. Y + 1) with Y = (int)(y
//     - 24), then, after compareToItem(cell, the cell one row up), the strip (+32 .. +65 of the clipped
//     x0, Y .. Y + buySellVertSpacing), then compareToItem(cell, the cell one column right).
//  3. A decoration is drawn when column % 3 == 0 and the row is even: iRandState becomes
//     (col * 12012 + row * 0x3cc5b + player * 0x352927) * 0x19660d + 0x3c6ef35f, and the texture
//     "inventrybox%d.tex" of ((iRandState >> 16) % 3) + 1 from "inventry.lmp" is drawn at the strip's
//     corner with textureDrawSuperColor(tex, x, y, 0, 0x80808080, 0, 105, 640, 512, 0, 0); column 6 also
//     draws "inventryboxend.tex" 96 pixels to the right.
//  4. A cell whose word +0 is zero ends here. same = 1; with row nonzero same = (compareToItem(row
//     above, cell) == 0); with column nonzero a nonzero compareToItem(column left, cell) clears it.
//  5. The picture rectangle is (screen[0] + 32*col + 17, Y' + 9) to (screen[0] + 32*col + cellW + 15,
//     Y' + cellH + 7) with Y' = (int)(y - 30), clipped; when the mouse lies within it (x0 <= mx <= x1 + 2,
//     y0 - 4 <= my <= y1 - 2) hover is set, drewBitmap[player] = 1 and the brightness becomes
//     (int)(isin(drawFrameNm << 10) * 32 + 140), else 128.
//  6. The picture is drawn at (screen[0] + (cellW / 2 - width / 2) + 32 * col + 15,
//     (cellH - height) / 2 + 6 + Y'). If itemEquipped(player, item p88 + 72 * halfword(cell(col,row) + 34))
//     is nonzero: when same, rectangleDraw(rect, 1, 0x20008000) then the picture. Otherwise
//     inventoryCheckWieldingStatus(player, cell) selects rectangleDraw colour 0x20000080 (nonzero) or
//     0x20445558, and only when same. The picture call is textureDrawSuperColor(pic, x, y, 5,
//     0x80000000 | brightness * 0x010101, 0, 102, 640, 600, 0, 0); with hover shopDrawItemBitmapGlow(pic,
//     x, y) follows.
// Constants (bit patterns): 24.0, 30.0, 32.0, 140.0.
// Inferred: this draws one inventory or shop cell's item picture with the frame and highlight.
// Unknown: the compareToItem calls' results in step 2 (they are ignored), and the roles of the
// halfword at cell + 34.
extern "C" {
extern unsigned char player[];
extern unsigned char shopList[];
extern unsigned invGrid;
extern unsigned shopGrid;
extern int buySellVertSpacing;
extern int drawFrameNm;
extern unsigned iRandState;
extern int drewBitmap[];
void* getPic__4itemiPQ24item11description(int index, void* cell);
void shopClipGridCoordinates__FPiN30(int* x0, int* y0, int* x1, int* y1);
int compareToItem__Q24item11descriptionPQ24item11description(void* left, void* right);
int sprintf(char* out, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void* texture, int x, int y, int mode, unsigned color, int a, int b, int c, int d, char e, char f);
float isin__Fi(int angle);
int itemEquipped__FiPQ24item11description(int player, void* item);
int inventoryCheckWieldingStatus__FiPQ24item11description(int player, void* cell);
void rectangleDraw__FiiiiiUi(int x0, int y0, int x1, int y1, int mode, unsigned color);
void shopDrawItemBitmapGlow__FP8_textureii(void* texture, int x, int y);
}

#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_base;
#endif

namespace {
// Pointer words of the image are 32-bit; a host check maps them to offsets in one arena.
inline unsigned char* at(unsigned word) {
#ifdef NORRATH_HOST_ARENA
    return norrath_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
template <class T>
inline T load(const unsigned char* base, int offset) {
    T value;
    __builtin_memcpy(&value, base + offset, sizeof value);
    return value;
}
inline float floatFromBits(unsigned bits) {
    float value;
    __builtin_memcpy(&value, &bits, sizeof value);
    return value;
}
}

extern "C" void shopDrawItemBitmap__FiP12screenStructfPQ24item11descriptioniii(int side, void* screenRecord, float y, void* cellPointer, int column,
                                                                             int row, int shop) {
    unsigned char* screen = static_cast<unsigned char*>(screenRecord);
    unsigned char* cell = static_cast<unsigned char*>(cellPointer);
    const int playerIndex = load<int>(screen, 52);
    const int tab = load<int>(screen, 80);
    unsigned char* items = player + 328 + 23984 * playerIndex + 4616 * tab;
    unsigned char* grid = at(invGrid) + side * 0x23988 + tab * 0xbdd8;
    if (shop != 0) {
        items = shopList + 4616 * tab;
        grid = at(shopGrid) + side * 0x23988 + tab * 0xbdd8;
    }
    int cellWidth = 32;
    int cellHeight = 34;
    const int columnX = column * 32;
    void* picture = nullptr;
    int pictureWidth = 0;
    int pictureHeight = 0;
    if (load<unsigned>(cell, 0) != 0) {
        picture = getPic__4itemiPQ24item11description(load<int>(player + 23984 * playerIndex, 84), cell);
        pictureHeight = load<short>(static_cast<unsigned char*>(picture), 2);
        pictureWidth = load<short>(static_cast<unsigned char*>(picture), 0);
        cellWidth = (pictureWidth / 32 + 1) * 32;
        cellHeight = ((pictureHeight - 6) / 34 + 1) * 34;
    }

    int stripX = columnX + 14;
    const int stripY = static_cast<int>(y - floatFromBits(0x41c00000u));
    int box[4];
    box[0] = load<int>(screen, 0) + stripX + 1;
    box[1] = stripY;
    box[2] = load<int>(screen, 0) + stripX + 33;
    box[3] = stripY + 1;
    shopClipGridCoordinates__FPiN30(&box[0], &box[1], &box[2], &box[3]);
    unsigned char* thisCell = grid + 5400 * column + 72 * row;
    compareToItem__Q24item11descriptionPQ24item11description(thisCell, thisCell - 72);
    const int clippedX = box[0];
    box[2] = clippedX + 33;
    box[3] = stripY + buySellVertSpacing;
    box[0] = clippedX + 32;
    box[1] = stripY;
    shopClipGridCoordinates__FPiN30(&box[0], &box[1], &box[2], &box[3]);
    compareToItem__Q24item11descriptionPQ24item11description(thisCell, thisCell + 5400);

    const int left = load<int>(screen, 0) + stripX;
    box[0] = left + 1;
    box[1] = stripY;
    box[2] = left + 33;
    box[3] = stripY + 1;
    if (column % 3 == 0 && ((row - 1) & 1) != 0) {
        const unsigned seed = (static_cast<unsigned>(column * 12012 + row * 0x3cc5b) + static_cast<unsigned>(playerIndex * 0x352927)) * 0x19660du + 0x3c6ef35fu;
        iRandState = seed;
        char name[64];
        sprintf(name, "inventrybox%d.tex", static_cast<int>((seed >> 16) % 3u) + 1);
        void* texture = lumpFindResource__FPcT0(const_cast<char*>("inventry.lmp"), name);
        textureDrawSuperColor__FP8_textureiiiUiiiiicc(texture, box[0], box[1], 0, 0x80808080u, 0, 105, 640, 512, 0, 0);
        if (column == 6) {
            texture = lumpFindResource__FPcT0(const_cast<char*>("inventry.lmp"), const_cast<char*>("inventryboxend.tex"));
            textureDrawSuperColor__FP8_textureiiiUiiiiicc(texture, box[0] + 96, box[1], 0, 0x80808080u, 0, 105, 640, 512, 0, 0);
        }
    }
    if (load<unsigned>(cell, 0) == 0) return;

    bool same = true;
    if (row != 0) same = compareToItem__Q24item11descriptionPQ24item11description(grid + 5400 * column + 72 * row - 72, cell) == 0;
    if (column != 0) {
        if (compareToItem__Q24item11descriptionPQ24item11description(grid + 5400 * column + 72 * row - 5400, cell) != 0) same = false;
    }

    int brightness = 128;
    bool hover = false;
    const int top = static_cast<int>(y - floatFromBits(0x41f00000u));
    const int offsetY = (cellHeight - pictureHeight) / 2;
    box[0] = load<int>(screen, 0) + columnX + 17;
    box[1] = top + 9;
    box[2] = load<int>(screen, 0) + cellWidth + columnX + 15;
    box[3] = top + 9 + cellHeight - 2;
    const int centreX = cellWidth / 2 - pictureWidth / 2;
    shopClipGridCoordinates__FPiN30(&box[0], &box[1], &box[2], &box[3]);
    const float mouseX = load<float>(screen, 68);
    const float mouseY = load<float>(screen, 72);
    if (static_cast<float>(box[0]) <= mouseX && mouseX <= static_cast<float>(box[2] + 2) && static_cast<float>(box[1] - 4) <= mouseY &&
        mouseY <= static_cast<float>(box[3] - 2)) {
        hover = true;
        drewBitmap[playerIndex] = 1;
        brightness = static_cast<int>(isin__Fi(drawFrameNm << 10) * floatFromBits(0x42000000u) + floatFromBits(0x430c0000u));
    }
    const int pictureX = load<int>(screen, 0) + (centreX + columnX) + 15;
    const int pictureY = offsetY + 6 + top;
    unsigned char* thisItem = items + 72 * load<short>(thisCell, 34);
    const unsigned brightColor = static_cast<unsigned>(brightness) << 16 | static_cast<unsigned>(brightness) << 8 | static_cast<unsigned>(brightness) | 0x80000000u;
    if (itemEquipped__FiPQ24item11description(playerIndex, thisItem) != 0) {
        if (!same) return;
        rectangleDraw__FiiiiiUi(box[0], box[1], box[2], box[3], 1, 0x20008000u);
        textureDrawSuperColor__FP8_textureiiiUiiiiicc(picture, pictureX, pictureY, 5, brightColor, 0, 102, 640, 600, 0, 0);
        if (hover) shopDrawItemBitmapGlow__FP8_textureii(picture, pictureX, pictureY);
        return;
    }
    const int wielding = inventoryCheckWieldingStatus__FiPQ24item11description(playerIndex, cell);
    if (!same) return;
    if (wielding != 0) rectangleDraw__FiiiiiUi(box[0], box[1], box[2], box[3], 1, 0x20000080u);
    else rectangleDraw__FiiiiiUi(box[0], box[1], box[2], box[3], 1, 0x20445558u);
    textureDrawSuperColor__FP8_textureiiiUiiiiicc(picture, pictureX, pictureY, 5, brightColor, 0, 102, 640, 600, 0, 0);
    if (hover) shopDrawItemBitmapGlow__FP8_textureii(picture, pictureX, pictureY);
}
