#include "../w9-animation-object-c-common/mem.hpp"
// Observed hudRunSlider__FiiiiiUifPii at 0x001c3a00, 448 bytes, 96-byte frame. Arguments: x, y, width, height (four
// ints), a fifth int that is forwarded to every rectangle, a colour (unsigned), a fraction (float), a pointer to the
// result word and a player index. It draws a horizontal slider and then reads the pad:
//  - px = (int)((float)(width - 20) * fraction) (truncated), cy = y + height / 2 (division by two rounds toward zero).
//  - Four calls to rectangleDraw__FiiiiiUi(left, top, right, bottom, fifth, colour), in this order:
//      handle  (x + 6 + px, cy - 12, x + 14 + px, cy + 12)  colour 0x80606060,
//      shadow  (x + 7 + px, cy - 11, x + 15 + px, cy + 13)  colour 0x40000000,
//      track   (x + 10, cy - 1, x + width - 10, cy + 1)     the colour with only its top byte kept,
//      frame   (x, y, x + width, y + height)                 the colour as given.
//    That is four calls in all; the handle and shadow share the x offset px.
//  - The result word is set to 0. When the player index is not -1 the word at pad + 16 + 88 * player is read: if
//    any bit of 0x00408000 is set the result is -1; the word is then read again and if any bit of 0x00802000 is set
//    the result becomes 1. The function returns the result word.
// Inferred: a result of -1 or 1 means left or right was pressed this frame for that player; the bit masks are two
// pad button groups (stick or d-pad). No button names are claimed.
// Unknown: the pad word's field name, which of the masks is which button, and why the fifth int is forwarded.
extern "C" {
extern unsigned char pad[];
void rectangleDraw__FiiiiiUi(int x0, int y0, int x1, int y1, int fifth, unsigned color);

int hudRunSlider__FiiiiiUifPii(int x, int y, int width, int height, int fifth, unsigned color, float fraction,
                               int* result, int player) {
    using namespace norrath_aoc;
    const int px = static_cast<int>(static_cast<float>(width - 20) * fraction);
    const int cy = y + height / 2;
    rectangleDraw__FiiiiiUi(x + 6 + px, cy - 12, x + 14 + px, cy + 12, fifth, 0x80606060u);
    rectangleDraw__FiiiiiUi(x + 7 + px, cy - 11, x + 15 + px, cy + 13, fifth, 0x40000000u);
    rectangleDraw__FiiiiiUi(x + 10, cy - 1, x + width - 10, cy + 1, fifth, (color >> 24) << 24);
    rectangleDraw__FiiiiiUi(x, y, x + width, y + height, fifth, color);
    *result = 0;
    if (player != -1) {
        const unsigned char* word = pad + 16 + 88 * player;
        if (load_word(word) & 0x00408000u) *result = -1;
        if (load_word(word) & 0x00802000u) *result = 1;
    }
    return *result;
}
}
