// Observed shopInventoryGridDraw__FiiiP12screenStruct at 0x001b82f8, 1840 bytes. Arguments: player,
// mode (0 = shop grid, nonzero = inventory grid), side (0 or 1, the screen half) and a screen record
// (ints and floats; words used: [0] x, [9], [10] scroll, [12], [17] and [18] cursor x and y as
// floats, [20] tab, [22], [25] held item description). A negative player returns at once and draws
// nothing, not even the task bar.
//  1. buySellVertSpacing becomes 34, localRandState becomes player * 0xe171f and itemInfoHeight[0..3]
//     become 0. hideAbilityAdjustMouse[side] is 0, or 1 when the byte at player record + 240 is
//     nonzero and tab is 5. shopDisableMouse[side] becomes 1 when multiHelpMessageOn[player] is
//     nonzero. The top line y0 is max(0, [9] - 68).
//  2. With shopDisableMouse[side], shopMouseDecay[side] and hideAbilityAdjustMouse[side] all zero the
//     mouse cursor is drawn from lump "inventry.lmp": "mousecursor.tex" when [22] is zero, else
//     "mousecursor2.tex", each twice with textureDrawFloat (second a shadow, y + 3.0, colour
//     0x34000000); in the second case the picked-up item picture (item::getPic of the record word
//     at +84 and [25]) is also drawn at (x + 22, y + 22) and (x + 22, y + 25) with textureDrawColor.
//  3. shopDisableMouse[side] becomes 0 (or 1 again when [12] is 1); a positive shopMouseDecay[side]
//     counts down by one.
//  4. Tab 3 runs runStats(side, 0), tab 4 runStats(side, 1), tab 5 runStats(side, 2 or 3 by byte 242
//     of the player record), then shopDrawTaskBar(screen). Other tabs draw the grid: the cursor
//     cell (column = (int)([17] - [0] - 17.0) / 32, at least -4; row = (int)([18] + top + [10] -
//     229.0) / 34, -1 when negative), a fade of 65 one-pixel rectangles (alpha ramping by 1.969..
//     up to 128) over the bottom, the "inventoryunderbar.tex" picture at x = side * 320, and 75 rows
//     of 9 cells (cell address = shopGrid or invGrid + tab * 0xbdd8 + side * 0x23988 + row * 72 +
//     column * 5400). Cells whose y is below 475 and above -50 are drawn with
//     shopDrawItemBitmap; the cursor cell also gets shopDrawInfoBox.
//  5. shopDrawTaskBar(screen) is called last.
// Inferred: the grid is a scrolling view of an item matrix with 75 rows of 9 columns for each tab.
// Unknown: iRandState's role (set to player * 0x3184b3 before the cell loop) and the screen words.
extern "C" {
extern int buySellVertSpacing;
extern int localRandState;
extern int iRandState;
extern int itemInfoHeight[4];
extern int hideAbilityAdjustMouse[2];
extern int shopDisableMouse[2];
extern int shopMouseDecay[2];
extern signed char multiHelpMessageOn[];
extern unsigned char player[];
extern unsigned char* rcursor;
extern unsigned shopGrid;
extern unsigned invGrid;
void* lumpFindResource__FPcT0(char* lump, char* name);
void textureDrawFloat__FP8_textureffiUiff(void* texture, float x, float y, int mode, unsigned color, float sx, float sy);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color);
void* getPic__4itemiPQ24item11description(int index, void* description);
void runStats__Fii(int side, int page);
void rectangleDraw__FiiiiiUi(int x1, int y1, int x2, int y2, int mode, unsigned color);
void shopDrawInfoBox__FiP12screenStructPQ24item11descriptioniiiii(int player, void* screen, void* cell, int mode, int a, int b, int c, int d);
void shopDrawItemBitmap__FiP12screenStructfPQ24item11descriptioniii(int side, void* screen, float y, void* cell, int column, int row, int flag);
void shopDrawTaskBar__FP12screenStruct(void* screen);
}

namespace {
inline void drawCursor(int* screen, const char* texture) {
    float* position = reinterpret_cast<float*>(screen);
    void* tex = lumpFindResource__FPcT0(const_cast<char*>("inventry.lmp"), const_cast<char*>(texture));
    textureDrawFloat__FP8_textureffiUiff(tex, position[17], position[18], 7, 0x80808080u, 1.0f, 1.0f);
    tex = lumpFindResource__FPcT0(const_cast<char*>("inventry.lmp"), const_cast<char*>(texture));
    textureDrawFloat__FP8_textureffiUiff(tex, position[17], position[18] + 3.0f, 7, 0x34000000u, 1.0f, 1.0f);
}
}

extern "C" void shopInventoryGridDraw__FiiiP12screenStruct(int playerNumber, int mode, int side, int* screen) {
    float* floats = reinterpret_cast<float*>(screen);
    if (playerNumber < 0) return;
    buySellVertSpacing = 34;
    localRandState = playerNumber * 0xe171f;
    for (int i = 3; i >= 0; --i) itemInfoHeight[i] = 0;
    int top = screen[9] - buySellVertSpacing * 2;
    hideAbilityAdjustMouse[side] = 0;
    if (top < 0) top = 0;
    if (player[playerNumber * 23984 + 240] != 0 && screen[20] == 5) hideAbilityAdjustMouse[side] = 1;
    if (multiHelpMessageOn[playerNumber] != 0) shopDisableMouse[side] = 1;
    if (shopDisableMouse[side] == 0 && shopMouseDecay[side] == 0 && hideAbilityAdjustMouse[side] == 0) {
        if (screen[22] == 0) {
            drawCursor(screen, "mousecursor.tex");
        } else {
            drawCursor(screen, "mousecursor2.tex");
            void* picture = getPic__4itemiPQ24item11description(*reinterpret_cast<int*>(player + playerNumber * 23984 + 84), reinterpret_cast<void*>(screen[25]));
            textureDrawColor__FP8_textureiiiUi(picture, static_cast<int>(floats[17] + 22.0f), static_cast<int>(floats[18] + 22.0f), 7, 0x80808080u);
            textureDrawColor__FP8_textureiiiUi(picture, static_cast<int>(floats[17] + 22.0f), static_cast<int>(floats[18] + 25.0f), 7, 0x34000000u);
        }
    }
    shopDisableMouse[side] = 0;
    if (shopMouseDecay[side] > 0) shopMouseDecay[side] = shopMouseDecay[side] - 1;
    if (screen[12] == 1) shopDisableMouse[side] = screen[12];
    const int tab = screen[20];
    if (tab == 3) {
        runStats__Fii(side, 0);
    } else if (tab == 4) {
        runStats__Fii(side, 1);
    } else if (tab == 5) {
        if (static_cast<signed char>(player[playerNumber * 23984 + 242]) == 0) runStats__Fii(side, 2);
        else runStats__Fii(side, 3);
    } else {
        const int half = static_cast<short>(*reinterpret_cast<unsigned short*>(rcursor + 2)) >> 1;
        const int columnValue = static_cast<int>(floats[17] - static_cast<float>(screen[0]) - 17.0f);
        const float lineBase = static_cast<float>(-half - top + 140);
        const int rowValue = static_cast<int>(floats[18] + lineBase + static_cast<float>(screen[10]) - 229.0f);
        int cursorColumn = columnValue / 32;
        int cursorRow = rowValue / 34;
        if (columnValue < -4) cursorColumn = -4;
        if (rowValue < 0) cursorRow = -1;
        float fade = 0.0f;
        for (int y = 413; y != 478;) {
            fade += 1.9692306518554688f;
            if (128.0f < fade) fade = 128.0f;
            if (fade < 0.0f) fade = 0.0f;
            if (y >= 93) rectangleDraw__FiiiiiUi(screen[0] + 15, y, screen[0] + 305, y + 1, 4, static_cast<unsigned>(static_cast<int>(fade)) << 24);
            ++y;
        }
        void* bar = lumpFindResource__FPcT0(const_cast<char*>("inventry.lmp"), const_cast<char*>("inventoryunderbar.tex"));
        textureDrawColor__FP8_textureiiiUi(bar, (side * 4 + side) << 6, 98, 5, 0x80808080u);
        iRandState = playerNumber * 0x3184b3;
        const unsigned sideOffset = static_cast<unsigned>(side) * 0x23988u;
        for (int row = 0; row < 75; ++row) {
            for (int column = 0; column < 9; ++column) {
                const unsigned base = mode == 0 ? shopGrid : invGrid;
                void* cell = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(
                    base + static_cast<unsigned>(screen[20]) * 0xbdd8u + sideOffset + static_cast<unsigned>(row * 72 + column * 5400)));
                const float y = static_cast<float>(-half - top + buySellVertSpacing * row + 140) - static_cast<float>(screen[10]);
                if (y < 475.0f && -50.0f < y) {
                    if (cursorRow == row && cursorColumn == column)
                        shopDrawInfoBox__FiP12screenStructPQ24item11descriptioniiiii(playerNumber, screen, cell, mode, 70, 128, static_cast<int>(floats[17]),
                                                                                       static_cast<int>(floats[18] + 32.0f));
                    shopDrawItemBitmap__FiP12screenStructfPQ24item11descriptioniii(side, screen, y, cell, column, row, mode == 0 ? 1 : 0);
                }
            }
        }
    }
    shopDrawTaskBar__FP12screenStruct(screen);
}
