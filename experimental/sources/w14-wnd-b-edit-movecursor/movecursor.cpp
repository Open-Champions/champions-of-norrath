// Observed: MoveCursor__9VIWndEditii at 0x010ad300 is 456 bytes. With (this, dx, dy); words are named by byte
// offset in this and mUI is the word _5VIWnd$mUI (0x01192c70). It first calls SoundPlay__4VIUI11VIUISoundId(mUI,
// 11). Then for axis 0 (delta dx) and axis 1 (delta dy), skipping an axis whose delta is zero:
//  - the float at +356 is set to zero, and when the 64-bit value at +328 (two words, x then y) differs from the
//    one at +336, the pair at +336 takes the pair at +328;
//  - the coordinate at +328 + 4 * axis is increased by the delta; it wraps when it reaches the limit of the
//    direction: for a negative delta the limit is -1 and the coordinate becomes 10 (axis 0) or 5 (axis 1); for a
//    non-negative delta the limit is 11 (axis 0) or 6 (axis 1) and the coordinate becomes 0. Each wrap copies the
//    pair at +328 to +336;
//  - while sKeyMap[x + 11 * y] (sKeyMap at 0x01192b80, signed byte) is zero, the same step (add the delta, wrap,
//    copy on wrap) repeats; with all cells of a line zero this never ends;
//  - for axis 0 only, afterwards, when x equals 10 the float at +388 is set to 0.25f (0x3e800000).
// Inferred: this steps the selected cell of the on-screen keyboard grid (11 columns by 6 rows) over empty cells with
// wrap-around; +336 keeps the cell the cursor came from (for the slide drawn from +356) and +388 is a key-repeat
// delay, matching the uses in OnDrawSelf and OnKeyDown of the same window.
// Unknown: field names, why only the x == 10 column shortens the repeat delay, and the return type (void).
extern "C" {
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int sound);
extern unsigned _5VIWnd$mUI;
extern unsigned char sKeyMap[];

void MoveCursor__9VIWndEditii(void* self, int dx, int dy) {
    int* words = static_cast<int*>(self);
    const int delta[2] = {dx, dy};
    SoundPlay__4VIUI11VIUISoundId(reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI)), 11);
    for (int axis = 0; axis < 2; ++axis) {
        const int step = delta[axis];
        if (step == 0) continue;
        int* position = words + 82;
        int* previous = words + 84;
        words[89] = 0;
        if (*reinterpret_cast<unsigned long long*>(position) != *reinterpret_cast<unsigned long long*>(previous)) {
            previous[0] = position[0];
            previous[1] = position[1];
        }
        position[axis] += step;
        int wrap;
        int limit;
        if (step < 0) {
            wrap = axis == 0 ? 10 : 5;
            limit = -1;
        } else {
            wrap = 0;
            limit = axis == 0 ? 11 : 6;
        }
        if (position[axis] == limit) {
            position[axis] = wrap;
            previous[0] = position[0];
            previous[1] = position[1];
        }
        while (static_cast<signed char>(sKeyMap[position[0] + 11 * position[1]]) == 0) {
            position[axis] += step;
            if (position[axis] == limit) {
                position[axis] = wrap;
                previous[0] = position[0];
                previous[1] = position[1];
            }
        }
        if (axis == 0 && position[0] == 10) {
            *reinterpret_cast<unsigned*>(words + 97) = 0x3e800000u;
        }
    }
}
}
