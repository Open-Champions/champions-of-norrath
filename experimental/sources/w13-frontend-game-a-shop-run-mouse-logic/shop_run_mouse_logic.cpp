// Observed shopRunMouseLogic__FiiP12screenStructi at 0x001b9e38, 412 bytes. Arguments: player pn,
// side (0 or 1, the screen half), a screen record and a scroll limit. Screen words used (the same
// numbering the shop grid code uses): [0] int left edge, [10] int scroll, [17] and [18] floats, the
// mouse position. pad is netLocalPad + 88 * pn, or zeroPad while gameControlPaused is set.
//  1. Returns at once when multiHelpMessageOn[pn] (a signed byte) is nonzero.
//  2. When shopDisableMouse[side] and hideAbilityAdjustMouse[side] are both zero the mouse position
//     moves by the pad floats at +20 and +24 times k1 (x gets +20, y gets +24).
//  3. x is limited to [float(screen[0]), float(screen[0] + 320)]; y to [0, 428]. (Observed: the
//     later scroll tests read y, not x.)
//  4. Scroll: [10] gains 2 for each of y > k2 and y > k3, and loses 2 for each of y < 112 and
//     y < k4; then it is limited to [0, the fourth argument].
// k1, k2, k3 and k4 are image constants with the bit patterns 0x40b309d8, 0x43be6665, 0x43d4cccc and
// 0x41b33332 (about 5.595, 380.8, 425.6 and 22.4).
// Inferred: this is the mouse-style cursor driven by the pad stick in the shop and inventory
// grids, scrolling the grid when the cursor is near its top or bottom edge. Unknown: what the
// constants are in screen units, and the name of the first two arguments' owner record.
extern "C" {
extern int gameControlPaused;
extern signed char multiHelpMessageOn[];
extern int shopDisableMouse[2];
extern int hideAbilityAdjustMouse[2];
extern unsigned char netLocalPad[];
extern unsigned char zeroPad[];
}

namespace {
inline float floatFromBits(unsigned bits) {
    float value;
    __builtin_memcpy(&value, &bits, sizeof value);
    return value;
}
}

extern "C" void shopRunMouseLogic__FiiP12screenStructi(int pn, int side, void* screen, int limit) {
    const unsigned char* pad = gameControlPaused != 0 ? zeroPad : netLocalPad + pn * 88;
    if (multiHelpMessageOn[pn] != 0) return;
    int* words = static_cast<int*>(screen);
    float* floats = static_cast<float*>(screen);
    if (shopDisableMouse[side] == 0 && hideAbilityAdjustMouse[side] == 0) {
        const float k1 = floatFromBits(0x40b309d8u);
        floats[17] = floats[17] + *reinterpret_cast<const float*>(pad + 20) * k1;
        floats[18] = floats[18] + *reinterpret_cast<const float*>(pad + 24) * k1;
    }
    const float left = static_cast<float>(words[0]);
    if (floats[17] < left) floats[17] = left;
    const float right = static_cast<float>(words[0] + 320);
    if (right < floats[17]) floats[17] = right;
    float y = floats[18];
    if (y < 0.0f) {
        floats[18] = 0.0f;
        y = 0.0f;
    }
    if (428.0f < y) {
        floats[18] = 428.0f;
        y = 428.0f;
    }
    if (floatFromBits(0x43be6665u) < y) words[10] += 2;
    if (floatFromBits(0x43d4ccccu) < y) words[10] += 2;
    if (y < 112.0f) words[10] -= 2;
    if (y < floatFromBits(0x41b33332u)) words[10] -= 2;
    if (words[10] < 0) words[10] = 0;
    if (limit < words[10]) words[10] = limit;
}
